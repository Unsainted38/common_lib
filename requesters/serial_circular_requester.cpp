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
    if(m_circularCommands.contains(cmd)) {
        return;
    }

    m_circularCommands.append(cmd);
}

void SerialCircularRequester::addDisposableCommand(AbstractCommand *cmd) {
    m_disposableCommands.enqueue(cmd);
}

void SerialCircularRequester::removeCommands() {
    qDeleteAll(m_circularCommands);
    m_circularCommands.clear();
}

void SerialCircularRequester::startRequest() {
    timer->start();
}

void SerialCircularRequester::stopRequest()
{
    timer->stop();
}

void SerialCircularRequester::processNext() {
    if(m_locker->isLocked() || m_waitingForWrite) {
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
    m_waitingForWrite = true;

#ifdef MYABSTRACTCONNECT_H
    m_connect->writeData(m_pendingPacket);
    onPacketAccepted(m_pendingPacket);
#else
    if (!m_transport->write(m_pendingPacket)) {
        m_waitingForWrite = false;
        m_pendingPacket.clear();
    }
#endif
}

void SerialCircularRequester::onPacketAccepted(const QByteArray &packet)
{
    if (!m_waitingForWrite || packet != m_pendingPacket) {
        return;
    }

    if (m_currentIsDisposable) {
        if (!m_disposableCommands.isEmpty() &&
            m_disposableCommands.head() == currentCmd) {
            m_disposableCommands.dequeue();
        }
        m_preferDisposable = false;
    } else if (!m_circularCommands.isEmpty()) {
        m_readIndex = (m_readIndex + 1) % m_circularCommands.size();
        m_preferDisposable = true;
    }

    m_waitingForWrite = false;
    m_pendingPacket.clear();
    m_locker->lock();
}

void SerialCircularRequester::unlock(QByteArray data) {
    bool tryParse = false;
    if (currentCmd) {
        tryParse = currentCmd->tryParse(data);
    }
    bool locked = m_locker->isLocked();
    if(locked && tryParse) {
        m_locker->unlock();
    }
}
