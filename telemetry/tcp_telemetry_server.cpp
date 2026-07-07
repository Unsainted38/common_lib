#include "tcp_telemetry_server.h"

void TcpTelemetryServer::loadConfig(const QString &configPath, const QString &section)
{
    QSettings settings(configPath, QSettings::IniFormat);
    settings.beginGroup(section);
    name = settings.value("Name", "Unknown telemetry server").toString();
    port = settings.value("Port", 1111).toUInt();
    settings.endGroup();
}



TcpTelemetryServer::TcpTelemetryServer(QString configPath, QString section, int period, ITelemetryPacketSource &source, QObject *parent)
    : QObject(parent),
    m_packetSource(source) {
    loadConfig(configPath, section);
    server = new QTcpServer();
    telemetryTimer = new QTimer();
    telemetryTimer->start(period);

    if(server->listen(QHostAddress::Any, port)) {
        qDebug() << name << "opened port: " << port;
    } else {
        qDebug() << "error while opening port: " << port;
    }

    connect(server, SIGNAL(newConnection()), this, SLOT(onNewClientConnection()));
    connect(telemetryTimer, SIGNAL(timeout()), this, SLOT(sendTelemetry()));
}

void TcpTelemetryServer::onNewClientConnection()
{
    QTcpSocket *newClient = server->nextPendingConnection();
    connect(newClient, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
    clients.append(newClient);
}

void TcpTelemetryServer::onClientDisconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    clients.removeAll(client);
    client->deleteLater();
}

void TcpTelemetryServer::sendTelemetry()
{
    const QByteArray packet = m_packetSource.makePacket();

    if (packet.isEmpty()) {
        return;
    }

    for (QTcpSocket *client : std::as_const(clients)) {
        if (client->state() == QAbstractSocket::ConnectedState) {
            client->write(packet);
        }
    }
}
