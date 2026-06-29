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
    QMutexLocker locker(&mutex);
    queue.enqueue(packet);
    QMetaObject::invokeMethod(this, "processQueue", Qt::QueuedConnection);
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
    QMutexLocker locker(&mutex);

    if(!queue.isEmpty()) {
        QByteArray packet = queue.dequeue();
        int writeCount = socket->write(packet);
        socket->flush();
        QString wErr = "w: " + QString::number(writeCount);

        if(writeCount < packet.length()) {
            emit translateError(wErr, WRITE_ERROR);
        } else {
            emit translateError(wErr, WRITE_OK);
        }
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
