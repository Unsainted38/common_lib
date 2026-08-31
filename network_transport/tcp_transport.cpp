#include "tcp_transport.h"

using namespace Qt;

TcpTransport::TcpTransport(QString configPath, QString section, QObject *parent)
    : AbstractNetworkTransport(configPath, section, parent) {
    loadConfig();
    setupTransport();
    open();
}

bool TcpTransport::write(const QByteArray &packet) {
    return writeTracked(packet) != 0;
}

quint64 TcpTransport::writeTracked(const QByteArray &packet) {
    if (packet.isEmpty()) {
        return 0;
    }

    quint64 packetId = 0;
    {
        QMutexLocker locker(&mutex);
        packetId = nextPacketId++;
        if (nextPacketId == 0) {
            nextPacketId = 1;
        }
        queue.enqueue({packetId, packet, 0, 0});
    }

    QMetaObject::invokeMethod(
        this,
        [this]() { processQueue(); },
        Qt::QueuedConnection);
    return packetId;
}

bool TcpTransport::open() {
    reconnectEnabled = true;

    if (socket->state() == QAbstractSocket::ConnectedState ||
        socket->state() == QAbstractSocket::ConnectingState ||
        socket->state() == QAbstractSocket::HostLookupState) {
        return true;
    }

    socket->connectToHost(hostAddress, port);
    qDebug() << "Connecting via TCP:"
             << hostAddress.toString() << port << name;
    return true;
}

bool TcpTransport::close() {
    reconnectEnabled = false;
    reconnectTimer->stop();
    heartbeatTimer->stop();
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
    connect(socket, &QTcpSocket::bytesWritten,
            this, &TcpTransport::onBytesWritten);
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
    if (socket->state() == QAbstractSocket::ConnectedState &&
        !heartbeatPacket.isEmpty()) {
        write(heartbeatPacket);
    }
}

void TcpTransport::reconnect() {
    if (!reconnectEnabled) {
        return;
    }

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
            // После разрыва невозможно определить, сколько байтов получил peer.
            // Повторяем текущий кадр целиком в новом TCP-соединении.
            queue.head().acceptedOffset = 0;
            queue.head().confirmedOffset = 0;
        }
    }

    resetTimers();
    if (reconnectEnabled) {
        scheduleReconnect();
    }
}

void TcpTransport::resetTimers() {
    heartbeatTimer->stop();
}

void TcpTransport::scheduleReconnect() {
    if (reconnectEnabled && !reconnectTimer->isActive()) {
        reconnectTimer->start();
    }
}

void TcpTransport::onErrorOccured(QAbstractSocket::SocketError error) {
    Q_UNUSED(error);
    qDebug() << "Socket error:" << socket->errorString();
    heartbeatTimer->stop();
    if (reconnectEnabled) {
        scheduleReconnect();
    }
}

void TcpTransport::processQueue() {
    QString writeResult;
    bool writeFailed = false;
    bool retryAfterDelay = false;

    {
        QMutexLocker locker(&mutex);

        if (queue.isEmpty() ||
            socket->state() != QAbstractSocket::ConnectedState) {
            return;
        }

        PendingPacket &pending = queue.head();
        const qsizetype remaining =
            pending.data.size() - pending.acceptedOffset;

        // Все байты уже находятся во внутреннем буфере QTcpSocket.
        // Удаление выполняется только из onBytesWritten().
        if (remaining == 0) {
            return;
        }

        const qint64 writeCount = socket->write(
            pending.data.constData() + pending.acceptedOffset,
            remaining);
        writeResult = "w: " + QString::number(writeCount);

        if (writeCount < 0) {
            writeFailed = true;
        } else {
            pending.acceptedOffset += static_cast<qsizetype>(writeCount);
            retryAfterDelay = writeCount == 0;
        }
    }

    socket->flush();

    if (writeFailed) {
        emit translateError(writeResult, WRITE_ERROR);
        // Новый TCP-поток должен повторить текущий кадр с первого байта.
        socket->abort();
        if (reconnectEnabled) {
            scheduleReconnect();
        }
        return;
    }

    if (retryAfterDelay) {
        QTimer::singleShot(50, this, [this]() { processQueue(); });
    }
}

void TcpTransport::onBytesWritten(qint64 count)
{
    quint64 acceptedPacketId = 0;
    QByteArray acceptedPacket;
    bool scheduleNext = false;
    bool continueCurrent = false;

    {
        QMutexLocker locker(&mutex);
        if (queue.isEmpty() || count <= 0) {
            return;
        }

        PendingPacket &pending = queue.head();
        const qsizetype unconfirmed =
            pending.acceptedOffset - pending.confirmedOffset;
        const qsizetype confirmedNow = qMin(
            static_cast<qsizetype>(count), unconfirmed);
        pending.confirmedOffset += confirmedNow;

        if (pending.acceptedOffset == pending.data.size() &&
            pending.confirmedOffset == pending.data.size()) {
            acceptedPacketId = pending.id;
            acceptedPacket = pending.data;
            queue.dequeue();
            scheduleNext = !queue.isEmpty();
        } else {
            continueCurrent = pending.acceptedOffset < pending.data.size();
        }
    }

    if (acceptedPacketId != 0) {
        emit translateError(
            "w: " + QString::number(acceptedPacket.size()), WRITE_OK);
        emit packetAccepted(acceptedPacketId, acceptedPacket);
    }

    if (scheduleNext || continueCurrent) {
        QMetaObject::invokeMethod(
            this,
            [this]() { processQueue(); },
            Qt::QueuedConnection);
    }
}

void TcpTransport::onReadSocket() {
    emit translateData(socket->readAll());
    lastActivity = QDateTime::currentDateTime();
}
