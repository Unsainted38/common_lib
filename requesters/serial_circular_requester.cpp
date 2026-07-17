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
    connect(m_connect, SIGNAL(readyToProcessData(QByteArray)), this, SLOT(unlock()), Qt::UniqueConnection);
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
    connect(m_transport, SIGNAL(translateData), this, SLOT(unlock()), Qt::UniqueConnection);
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

size_t SerialCircularRequester::currentMemoryUsage()
{
    mach_task_basic_info info;
    mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;

    if (task_info(mach_task_self(),
                  MACH_TASK_BASIC_INFO,
                  (task_info_t)&info,
                  &count) != KERN_SUCCESS)
        return 0;

    return info.resident_size;
}

void SerialCircularRequester::processNext() {
    if(m_locker->isLocked()) {
        return;
    }
    static bool concurent_flag = true;
    AbstractCommand *cmd = nullptr;

    if(!m_disposableCommands.isEmpty() && concurent_flag) {

        currentCmd = m_disposableCommands.dequeue();

        if(cmd) {
            m_locker->lock();
            concurent_flag = false;
#ifdef MYABSTRACTCONNECT_H
            m_connect->writeData(currentCmd->makeCommand());
#else
            m_transport->write(currentCmd->makeCommand());
#endif

        }
    } else if(!m_circularCommands.isEmpty()) {
        currentCmd = m_circularCommands[m_readIndex];
        m_readIndex = (m_readIndex + 1) % m_circularCommands.size();

        if(cmd) {
            m_locker->lock();
            concurent_flag = true;
#ifdef MYABSTRACTCONNECT_H
            m_connect->writeData(currentCmd->makeCommand());
#else
            m_transport->write(currentCmd->makeCommand());
#endif

        }
    }

}

void SerialCircularRequester::unlock(QByteArray data) {
    bool tryParse = currentCmd->tryParse(data);
    bool locked = m_locker->isLocked();
    if(locked && tryParse) {
        m_locker->unlock();
    }
}
