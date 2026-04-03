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
    memTimer = new QTimer(this);
    connect(memTimer, &QTimer::timeout, this, [this]() {

        qDebug()
        << "this" << this
        << "Memory MB:" << currentMemoryUsage()/1024.0/1024.0
        << "writeQueue:" << m_writeCommands.size()
        << "readCommands:" << m_readCommands.size();

    });
    memTimer->start(1000);

}
#else
SerialCircularRequester::SerialCircularRequester(AbstractNetworkTransport *transport, NetworkTransportLocker *locker, int pollIntervalMs, QObject *parent)
    : QObject(parent),
      m_transport(transport),
      m_locker(locker),
      timer(new QTimer(this)) {
    timer->setInterval(pollIntervalMs);
    connect(timer, &QTimer::timeout, this, &SerialCircularRequester::processNext);
    connect(m_transport, &AbstractNetworkTransport::translateData, this, &SerialCircularRequester::translateData);
    connect(m_transport, SIGNAL(translateData), this, SLOT(unlock()), Qt::UniqueConnection);
    memTimer = new QTimer(this);
    connect(memTimer, &QTimer::timeout, this, [this]() {

        qDebug()
        << "this" << this
        << "Memory MB:" << currentMemoryUsage()/1024.0/1024.0
        << "writeQueue:" << m_writeCommands.size()
        << "readCommands:" << m_readCommands.size();

    });
    memTimer->start(1000);
}
#endif

void SerialCircularRequester::addCommand(AbstractCommand *cmd) {
    if(m_readCommands.contains(cmd)) {
        return;
    }

    m_readCommands.append(cmd);
}

void SerialCircularRequester::addExtraCommand(AbstractCommand *cmd) {
    m_writeCommands.enqueue(cmd);
}

void SerialCircularRequester::removeCommands() {
    qDeleteAll(m_readCommands);
    m_readCommands.clear();
    //m_writeCommands.clear();
}

void SerialCircularRequester::startRequest() {
    timer->start();
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

    AbstractCommand *cmd = nullptr;

    if(!m_writeCommands.isEmpty()) {
        cmd = m_writeCommands.dequeue();

        if(cmd) {
            m_locker->lock();
#ifdef MYABSTRACTCONNECT_H
            m_connect->writeData(cmd->makeWriteCommand());
            // disconnect(m_connect, &MyAbstractConnect::readyToProcessData, this, &SerialCircularRequester::unlock);
            // connect(m_connect, &MyAbstractConnect::readyToProcessData, this, &SerialCircularRequester::unlock);
#else
            m_transport->write(cmd->makeWriteCommand());
            //disconnect(m_transport, &AbstractNetworkTransport::translateData, this, &SerialCircularRequester::unlock);
            //connect(m_transport, &AbstractNetworkTransport::translateData, this, &SerialCircularRequester::unlock);
#endif

        }
    } else if(!m_readCommands.isEmpty()) {
        cmd = m_readCommands[m_readIndex];
        m_readIndex = (m_readIndex + 1) % m_readCommands.size();

        if(cmd) {
            m_locker->lock();
#ifdef MYABSTRACTCONNECT_H
            m_connect->writeData(cmd->makeReadCommand());
            // disconnect(m_connect, &MyAbstractConnect::readyToProcessData, this, &SerialCircularRequester::unlock);
            // connect(m_connect,  &MyAbstractConnect::readyToProcessData, this, &SerialCircularRequester::unlock);
#else
            m_transport->write(cmd->makeReadCommand());
            //disconnect(m_transport, &AbstractNetworkTransport::translateData, this, &SerialCircularRequester::unlock);
            //connect(m_transport, &AbstractNetworkTransport::translateData, this, &SerialCircularRequester::unlock);
#endif

        }
    }

}

void SerialCircularRequester::unlock() {
    if(m_locker->isLocked()) {
        m_locker->unlock();
    }
#ifdef MYABSTRACTCONNECT_H
    //disconnect(m_connect, &MyAbstractConnect::readyToProcessData, this, &SerialCircularRequester::unlock);
#else
    //disconnect(m_transport, &AbstractNetworkTransport::translateData, this, &SerialCircularRequester::unlock);
#endif
}
