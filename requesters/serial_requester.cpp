#include "serial_requester.h"

SerialRequester::SerialRequester(AbstractNetworkTransport *transport, NetworkTransportLocker locker, QObject *parent)
    : QObject(parent), m_transport(transport), m_locker(locker) {
    //    connect(m_transport, SIGNAL(translateData(QByteArray)),
    //            this, SLOT(onDataReceived(QByteArray)));

    //    //    connect(&parser, SIGNAL(dataReady(QByteArray)),
    //    //            this, SLOT(onParsedData(QByteArray)));

    //    connect(&timeoutTimer, SIGNAL(timeout()),
    //            this, SLOT(onTimeout()));

    //    timeoutTimer.setSingleShot(true);
}

void SerialRequester::enqueue(const QByteArray &command,
                              std::function<void(const QByteArray &)> onReply,
                              int timeoutMs) {
    queue.enqueue({command, onReply, timeoutMs});

    if(!busy && !m_locker.isLocked()) {
        processNext();
    }
}

void SerialRequester::enqueueConcurrent(const QByteArray &command, std::function<void (const QByteArray &)> onReply, int timeoutMs) {
    queue.push_front({command, onReply, timeoutMs});

    if(!busy && !m_locker.isLocked()) {
        m_concurrentFlag = true;
        processNext();
    }
}

void SerialRequester::startRequest() {
    if(queue.isEmpty() || m_locker.isLocked()) {
        return;
    }

    busy = true;
}

void SerialRequester::processNext() {
    if(queue.isEmpty() || m_locker.isLocked()) {
        return;
    }

    busy = true;

    if(m_concurrentFlag) {
        current = queue.dequeue();
        m_concurrentFlag = false;
    } else {
        current = queue.dequeue();
        queue.enqueue(current);
    }

    m_locker.lock();
    m_transport->write(current.command);
    timeoutTimer.start(current.timeoutMs);
}

void SerialRequester::onDataReceived(const QByteArray &data) {

    //parser.parseReply(data);
}

void SerialRequester::onParsedData(const QByteArray &data) {
    timeoutTimer.stop();
    m_locker.unlock();
    busy = false;

    if(current.onReply) {
        current.onReply(data);
        emit translateData(data);
    }

    processNext();
}

void SerialRequester::onTimeout() {
    m_locker.unlock();
    busy = false;

    if(current.onReply) {
        current.onReply(QByteArray());
    }

    processNext();
}
