#include "udp_transport.h"

using namespace Qt;

UdpTransport::UdpTransport(QString configPath, QString section, QObject *parent)
    : AbstractNetworkTransport(configPath, section, parent) {
    loadConfig();
    setupTransport();
    open();
}


void UdpTransport::loadConfig()
{
    QSettings settings(m_configPath, QSettings::IniFormat);
    settings.beginGroup(m_section);
    hostAddress = QHostAddress(settings.value("HostIp", QHostAddress::LocalHost).toString());
    port = settings.value("Port", 7777).toUInt();
    name = settings.value("Name", "udp_client").toString();
    listenIp = QHostAddress(settings.value("ListenIp", QHostAddress::Any).toString());
}

void UdpTransport::setupTransport()
{
    socket = new QUdpSocket(this);
    heartbeatTimer = new QTimer(this);
    heartbeatTimer->setInterval(5000);

    reconnectTimer = new QTimer(this);
    reconnectTimer->setInterval(3000);
    reconnectTimer->setSingleShot(true);

    connect(socket, &QUdpSocket::connected, this, &UdpTransport::onConnected);
    connect(socket, &QUdpSocket::disconnected, this, &UdpTransport::onDisconnected);
    connect(socket, &QUdpSocket::errorOccurred, this, &UdpTransport::onErrorOccured);
    connect(socket, &QUdpSocket::bytesWritten, this, [this](qint64) {
        QMetaObject::invokeMethod(
            this,
            [this]() { processQueue(); },
            Qt::QueuedConnection);
    });
    connect(heartbeatTimer, &QTimer::timeout, this, &UdpTransport::heartbeat);
    connect(socket, &QUdpSocket::readyRead, this, &UdpTransport::onReadSocket);
    connect(reconnectTimer, &QTimer::timeout, this, &UdpTransport::reconnect);

    lastActivity = QDateTime::currentDateTime();
}

bool UdpTransport::open()
{
    socket->connectToHost(hostAddress, port);

    if(!socket->isOpen()) {
        qDebug() << "error while connect to udp port:" << endl
                 << "Ip:" << hostAddress.toString() << endl
                 << "Port:" << port << endl
                 << "Name:" << name << endl
                 << socket->errorString() << endl
                 << "trying to reconnect in 5 sec" << endl;
        QTimer::singleShot(5000, this, "UdpTransport::open");
        return false;
    }

    qDebug() << "connected via udp: " << endl
             << "Ip:" << hostAddress.toString() << endl
             << "Port:" << port << endl
             << "Name:" << name << endl;
    return true;
}

bool UdpTransport::write(const QByteArray &packet)
{
    {
        QMutexLocker locker(&mutex);
        queue.enqueue(packet);
    }

    QMetaObject::invokeMethod(
        this,
        [this]() { processQueue(); },
        Qt::QueuedConnection);
    return true;
}

bool UdpTransport::close()
{
    socket->disconnectFromHost();
    return true;
}



void UdpTransport::heartbeat()
{
    if(socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(heartbeatPacket); // ISSUE Придумать, как пихать в сокет хартбит пакет
        socket->flush();
    }
}

void UdpTransport::onConnected()
{
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

void UdpTransport::onDisconnected()
{
    qDebug() << "Disconnected from" << hostAddress.toString() << ":" << port;

    if(connectedState) {
        connectedState = false;
        //emit connectedChanged(false);
    }

    resetTimers();
    scheduleReconnect();
}

void UdpTransport::onErrorOccured(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);
    qDebug() << "Socket error:" << socket->errorString();
    heartbeatTimer->stop();
    scheduleReconnect();
}

void UdpTransport::processQueue()
{
    QByteArray acceptedPacket;
    QString writeResult;
    bool packetWasAccepted = false;
    bool writeFailed = false;
    bool scheduleNext = false;

    {
        QMutexLocker locker(&mutex);

        if (queue.isEmpty() ||
            socket->state() != QAbstractSocket::ConnectedState) {
            return;
        }

        const QByteArray &packet = queue.head();
        const qint64 writeCount = socket->write(packet);
        writeResult = "w: " + QString::number(writeCount);

        if (writeCount == packet.size()) {
            acceptedPacket = packet;
            packetWasAccepted = true;
            queue.dequeue();
            scheduleNext = !queue.isEmpty();
        } else {
            // UDP-дейтаграмма атомарна: при ошибке сохраняем её целиком.
            writeFailed = true;
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
    }
}

void UdpTransport::onReadSocket()
{
    emit translateData(socket->readAll());
    lastActivity = QDateTime::currentDateTime();
}

void UdpTransport::resetTimers()
{
    heartbeatTimer->stop();
}

void UdpTransport::scheduleReconnect()
{
    if(!reconnectTimer->isActive()) {
        reconnectTimer->start();
    }
}

void UdpTransport::reconnect()
{
    if(socket->state() == QAbstractSocket::ConnectedState ||
        socket->state() == QAbstractSocket::ConnectingState) {
        return;
    }

    qDebug() << "Trying to reconnect to:" << hostAddress.toString() << port;
    socket->abort();
    socket->connectToHost(hostAddress, port);
}
