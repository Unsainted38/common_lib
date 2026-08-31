#include "tcp_transport.h"

using namespace Qt;

TcpTransport::TcpTransport(QString configPath, QString section, QObject *parent)
    : AbstractNetworkTransport(configPath, section, parent) {
    loadConfig();
    setupTransport();
    open();
}

bool TcpTransport::write(const QByteArray &packet) {
    {
        QMutexLocker locker(&mutex);
        queue.enqueue({packet, 0});
    }

    QMetaObject::invokeMethod(
        this,
        [this]() { processQueue(); },
        Qt::QueuedConnection);
    return true;
}

bool TcpTransport::open() {
    socket->connectToHost(hostAddress, port);

    if(!socket->isOpen()) {
        qDebug() << "error while connect to tcp port:" << endl
                 << "Ip:" << hostAddress.toString() << endl
                 << "Port:" << port << endl
                 << "Name:" << name << endl
                 << socket->errorString() << endl
                 << "trying to reconnect in 5 sec" << endl;
        QTimer::singleShot(5000, this, "TcpTransport::open");
        return false;
    }

    qDebug() << "connected via tcp: " << endl
             << "Ip:" << hostAddress.toString() << endl
             << "Port:" << port << endl
             << "Name:" << name << endl;
    return true;
}

bool TcpTransport::close() {
    socket->disconnectFromHost();
    return true;
}

void TcpTransport::setupTransport() {
    socket = new QTcpSocket(this);
    heartbeatTimer = new QTimer(this);
    heartbeatTimer->setInterval(5000);

    reconnectTimer = new QTimer(this);
    reconnectTimer->setInterval(3000);
    reconnectTimer->setSingleShot(true);


    //    connect(socket, SIGNAL(connected()), this, SLOT(onConnected()));
    //    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    //    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onErrorOccured(QAbstractSocket::SocketError error)));
    //    connect(heartbeatTimer, SIGNAL(timeout()), this, SLOT(heartbeat()));
    //    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadSocket()));
    //    connect(reconnectTimer, SIGNAL(timeout()), this, SLOT(reconnect()));

    connect(socket, &QTcpSocket::connected, this, &TcpTransport::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &TcpTransport::onDisconnected);
    connect(socket, &QTcpSocket::errorOccurred, this, &TcpTransport::onErrorOccured);
    connect(socket, &QTcpSocket::bytesWritten, this, [this](qint64) {
        QMetaObject::invokeMethod(
            this,
            [this]() { processQueue(); },
            Qt::QueuedConnection);
    });
    connect(heartbeatTimer, &QTimer::timeout, this, &TcpTransport::heartbeat);
    connect(socket, &QTcpSocket::readyRead, this, &TcpTransport::onReadSocket);
    connect(reconnectTimer, &QTimer::timeout, this, &TcpTransport::reconnect);

    lastActivity = QDateTime::currentDateTime();
}

void TcpTransport::loadConfig() {
    QSettings settings(m_configPath, QSettings::IniFormat);
    settings.beginGroup(m_section);
    hostAddress = QHostAddress(settings.value("HostIp", QHostAddress::LocalHost).toString());
    port = settings.value("Port", 7777).toUInt();
    name = settings.value("Name", "tcp_client").toString();
    listenIp = QHostAddress(settings.value("ListenIp", QHostAddress::Any).toString());
}

void TcpTransport::heartbeat() {
    if(socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(heartbeatPacket); // ISSUE Придумать, как пихать в сокет хартбит пакет
        socket->flush();
    }
}

void TcpTransport::reconnect() {
    if(socket->state() == QAbstractSocket::ConnectedState ||
            socket->state() == QAbstractSocket::ConnectingState) {
        return;
    }

    qDebug() << "Trying to reconnect to:" << hostAddress.toString() << port;
    socket->abort();
    socket->connectToHost(hostAddress, port);
}

void TcpTransport::onConnected() {
    qDebug() << "Connected to" << hostAddress.toString() << ":" << port;
    connectedState = true;
    //emit connectedChanged(true);
    lastActivity = QDateTime::currentDateTime();

    heartbeatTimer->start();

    QMetaObject::invokeMethod(
        this,
        [this]() { processQueue(); },
        Qt::QueuedConnection);
}

void TcpTransport::onDisconnected() {
    qDebug() << "Disconnected from" << hostAddress.toString() << ":" << port;

    if(connectedState) {
        connectedState = false;
        //emit connectedChanged(false);
    }

    // Частично записанный TCP-кадр нельзя продолжать в новом соединении:
    // новый поток должен получить его с первого байта.
    {
        QMutexLocker locker(&mutex);
        if (!queue.isEmpty()) {
            queue.head().offset = 0;
        }
    }

    resetTimers();
    scheduleReconnect();
}

void TcpTransport::resetTimers() {
    heartbeatTimer->stop();
}

void TcpTransport::scheduleReconnect() {
    if(!reconnectTimer->isActive()) {
        reconnectTimer->start();
    }
}

void TcpTransport::onErrorOccured(QAbstractSocket::SocketError error) {
    Q_UNUSED(error);
    qDebug() << "Socket error:" << socket->errorString();
    heartbeatTimer->stop();
    scheduleReconnect();
}

void TcpTransport::processQueue() {
    QByteArray acceptedPacket;
    QString writeResult;
    bool packetWasAccepted = false;
    bool writeFailed = false;
    bool retryAfterDelay = false;
    bool scheduleNext = false;

    {
        QMutexLocker locker(&mutex);

        if (queue.isEmpty() ||
            socket->state() != QAbstractSocket::ConnectedState) {
            return;
        }

        PendingPacket &pending = queue.head();
        const qsizetype remaining = pending.data.size() - pending.offset;

        if (remaining == 0) {
            acceptedPacket = pending.data;
            packetWasAccepted = true;
            queue.dequeue();
            scheduleNext = !queue.isEmpty();
        } else {
            const qint64 writeCount = socket->write(
                pending.data.constData() + pending.offset,
                remaining);
            writeResult = "w: " + QString::number(writeCount);

            if (writeCount < 0) {
                writeFailed = true;
            } else {
                pending.offset += static_cast<qsizetype>(writeCount);
                retryAfterDelay = writeCount == 0;

                if (pending.offset == pending.data.size()) {
                    acceptedPacket = pending.data;
                    packetWasAccepted = true;
                    queue.dequeue();
                    scheduleNext = !queue.isEmpty();
                }
            }
        }
    }

    socket->flush();

    if (writeFailed) {
        emit translateError(writeResult, WRITE_ERROR);
        QTimer::singleShot(100, this, [this]() { processQueue(); });
        return;
    }

    if (packetWasAccepted) {
        emit translateError(writeResult, WRITE_OK);
        emit packetAccepted(acceptedPacket);
    }

    if (scheduleNext) {
        QMetaObject::invokeMethod(
            this,
            [this]() { processQueue(); },
            Qt::QueuedConnection);
    } else if (retryAfterDelay) {
        QTimer::singleShot(50, this, [this]() { processQueue(); });
    }
}

void TcpTransport::onReadSocket() {
    emit translateData(socket->readAll());
    lastActivity = QDateTime::currentDateTime();
}
