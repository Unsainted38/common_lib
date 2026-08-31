#include "serial_circular_requester.h"
#ifdef MYABSTRACTCONNECT_H
SerialCircularRequester::SerialCircularRequester(MyAbstractConnect *transport, NetworkTransportLocker *locker, int pollIntervalMs, QObject *parent)
    : QObject(parent),
      m_connect(transport),
      timer(new QTimer(this)),
      m_locker(locker) {
    timer->setInterval(pollIntervalMs);
    connect(timer, &QTimer::timeout, this, &SerialCircularRequester::processNext);
    connect(m_connect, &MyAbstractConnect::readyToProcessData, this, &SerialCircularRequester::translateData);
    connect(m_connect, SIGNAL(readyToProcessData(QByteArray)), this, SLOT(unlock(QByteArray)), Qt::UniqueConnection);
}

MyAbstractConnect *SerialCircularRequester::getTransport()
{
    return m_connect;
}
#else
SerialCircularRequester::SerialCircularRequester(AbstractNetworkTransport *transport, NetworkTransportLocker *locker, int pollIntervalMs, QObject *parent)
    : QObject(parent),
      m_transport(transport),
      timer(new QTimer(this)),
      m_locker(locker) {
    timer->setInterval(pollIntervalMs);
    connect(timer, &QTimer::timeout, this, &SerialCircularRequester::processNext);
    connect(m_transport, &AbstractNetworkTransport::translateData, this, &SerialCircularRequester::translateData);
    connect(m_transport, SIGNAL(translateData(QByteArray)), this, SLOT(unlock(QByteArray)), Qt::UniqueConnection);
    connect(m_transport, &AbstractNetworkTransport::packetAccepted,
            this, &SerialCircularRequester::onPacketAccepted,
            Qt::UniqueConnection);
}

AbstractNetworkTransport *SerialCircularRequester::getTransport()
{
    return m_transport;
}
#endif

void SerialCircularRequester::addCircularCommand(AbstractCommand *cmd) {
    if (!cmd || m_circularCommands.contains(cmd)) {
        return;
    }

    m_circularCommands.append(cmd);
}

void SerialCircularRequester::addDisposableCommand(AbstractCommand *cmd) {
    // Команды устройств являются переиспользуемыми объектами. Повторное
    // добавление того же указателя до отправки не должно создавать несколько
    // одинаковых записей, каждая из которых всё равно увидит последнее value.
    if (cmd && !m_disposableCommands.contains(cmd)) {
        m_disposableCommands.enqueue(cmd);
    }
}

void SerialCircularRequester::removeCircularCommand(AbstractCommand *cmd)
{
    const qsizetype index = m_circularCommands.indexOf(cmd);
    if (index < 0) {
        return;
    }

    m_circularCommands.removeAt(index);
    m_disposableCommands.removeAll(cmd);
    if (index < m_readIndex) {
        --m_readIndex;
    }
    if (m_readIndex < 0 || m_readIndex >= m_circularCommands.size()) {
        m_readIndex = 0;
    }

    if (currentCmd == cmd && m_state != RequestState::Idle) {
        m_deleteCurrentWhenIdle = true;
    } else {
        delete cmd;
        if (currentCmd == cmd) {
            currentCmd = nullptr;
        }
    }
}

void SerialCircularRequester::removeCommands() {
    const AbstractCommand *activeCommand = currentCmd.data();
    const bool activeIsCircular =
        activeCommand && m_circularCommands.contains(currentCmd.data());

    for (AbstractCommand *command : std::as_const(m_circularCommands)) {
        if (command != activeCommand) {
            delete command;
        }
    }
    m_circularCommands.clear();
    m_readIndex = 0;

    if (activeIsCircular) {
        if (m_state == RequestState::Idle) {
            delete currentCmd.data();
            currentCmd = nullptr;
        } else {
            // Активная команда удалится после ответа или таймаута.
            m_deleteCurrentWhenIdle = true;
        }
    }
}

void SerialCircularRequester::startRequest() {
    timer->start();
}

void SerialCircularRequester::stopRequest()
{
    timer->stop();
}

void SerialCircularRequester::processNext() {
    if (m_state == RequestState::WaitingForWrite) {
        return;
    }

    if (m_state == RequestState::WaitingForResponse) {
        if (m_responseTimer.isValid() &&
            m_responseTimer.elapsed() < m_locker->timeout()) {
            return;
        }

        qWarning() << "Command response timeout";
        m_locker->unlock();
        finishCurrentCommand();
    }

    if (m_locker->isLocked()) {
        return;
    }

    while (!m_disposableCommands.isEmpty() &&
           m_disposableCommands.head() == nullptr) {
        m_disposableCommands.dequeue();
    }

    const bool takeDisposable =
        !m_disposableCommands.isEmpty() &&
        (m_preferDisposable || m_circularCommands.isEmpty());

    if (takeDisposable) {
        currentCmd = m_disposableCommands.head();
        m_currentIsDisposable = true;
    } else if (!m_circularCommands.isEmpty()) {
        if (m_readIndex >= m_circularCommands.size()) {
            m_readIndex = 0;
        }

        currentCmd = m_circularCommands.at(m_readIndex);
        m_currentIsDisposable = false;
    } else {
        currentCmd = nullptr;
        return;
    }

    if (!currentCmd) {
        if (!m_currentIsDisposable) {
            m_readIndex = (m_readIndex + 1) % m_circularCommands.size();
        }
        return;
    }

    m_pendingPacket = currentCmd->makeCommand();
    m_earlyResponseBuffer.clear();
    if (m_pendingPacket.isEmpty()) {
        qWarning() << "Command produced an empty packet";
        rejectCurrentCommand();
        return;
    }

    m_state = RequestState::WaitingForWrite;

#ifdef MYABSTRACTCONNECT_H
    m_pendingPacketId = 1;
    m_connect->writeData(m_pendingPacket);
    onPacketAccepted(m_pendingPacketId, m_pendingPacket);
#else
    m_pendingPacketId = m_transport->writeTracked(m_pendingPacket);
    if (m_pendingPacketId == 0) {
        m_state = RequestState::Idle;
        m_pendingPacket.clear();
        currentCmd = nullptr;
    }
#endif
}

void SerialCircularRequester::onPacketAccepted(
    quint64 packetId,
    const QByteArray &packet)
{
    if (m_state != RequestState::WaitingForWrite ||
        packetId != m_pendingPacketId ||
        packet != m_pendingPacket) {
        return;
    }

    if (m_currentIsDisposable) {
        if (!m_disposableCommands.isEmpty() &&
            m_disposableCommands.head() == currentCmd.data()) {
            m_disposableCommands.dequeue();
        }
        m_preferDisposable = false;
    } else if (!m_circularCommands.isEmpty()) {
        m_readIndex = (m_readIndex + 1) % m_circularCommands.size();
        m_preferDisposable = true;
    }

    m_state = RequestState::WaitingForResponse;
    m_pendingPacketId = 0;
    m_pendingPacket.clear();
    m_locker->lock();
    m_responseTimer.restart();

    if (!m_earlyResponseBuffer.isEmpty()) {
        const QByteArray earlyResponse = std::move(m_earlyResponseBuffer);
        unlock(earlyResponse);
    }
}

void SerialCircularRequester::unlock(QByteArray data) {
    if (m_state == RequestState::WaitingForWrite) {
        m_earlyResponseBuffer.append(data);
        return;
    }

    if (m_state != RequestState::WaitingForResponse) {
        return;
    }

    if (!currentCmd ||
        !m_responseTimer.isValid() ||
        m_responseTimer.elapsed() >= m_locker->timeout()) {
        m_locker->unlock();
        finishCurrentCommand();
        return;
    }

    if (currentCmd->tryParse(data)) {
        m_locker->unlock();
        finishCurrentCommand();
    }
}

void SerialCircularRequester::rejectCurrentCommand()
{
    if (m_currentIsDisposable) {
        if (!m_disposableCommands.isEmpty() &&
            m_disposableCommands.head() == currentCmd.data()) {
            m_disposableCommands.dequeue();
        }
        m_preferDisposable = false;
    } else if (!m_circularCommands.isEmpty()) {
        m_readIndex = (m_readIndex + 1) % m_circularCommands.size();
        m_preferDisposable = true;
    }

    finishCurrentCommand();
}

void SerialCircularRequester::finishCurrentCommand()
{
    AbstractCommand *finishedCommand = currentCmd.data();

    m_state = RequestState::Idle;
    m_pendingPacketId = 0;
    m_pendingPacket.clear();
    m_earlyResponseBuffer.clear();
    currentCmd = nullptr;
    m_responseTimer.invalidate();

    if (m_deleteCurrentWhenIdle) {
        m_deleteCurrentWhenIdle = false;
        delete finishedCommand;
    }
}
