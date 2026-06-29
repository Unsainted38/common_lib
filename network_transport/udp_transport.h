#ifndef UDP_TRANSPORT_H
#define UDP_TRANSPORT_H

#include <QObject>
#include "abstract_network_transport.h"
#include <QUdpSocket>
#include <QQueue>
#include <QTimer>
#include <QDateTime>


class UdpTransport : public AbstractNetworkTransport
{
    Q_OBJECT
    QUdpSocket *socket;
    QHostAddress hostAddress;
    quint16 port = 7777;
    QHostAddress listenIp;
    QString name = "udp_client";
    QMutex mutex;
    QQueue<QByteArray> queue;
    QTimer *heartbeatTimer;
    QTimer *reconnectTimer;
    bool connectedState = false;
    QDateTime lastActivity;
    QByteArray heartbeatPacket;

public:
    explicit UdpTransport(QString configPath, QString section, QObject *parent = nullptr);

    // AbstractNetworkTransport interface
public:
    void loadConfig() override;
    void setupTransport() override;
    bool open() override;
    bool write(const QByteArray &packet) override;
    bool close() override;
    void heartbeat() override;

private:
    void onConnected();
    void onDisconnected();
    void onErrorOccured(QAbstractSocket::SocketError error);
    void processQueue() override;
    void onReadSocket();
    void resetTimers();
    void scheduleReconnect();
    void reconnect();
};

#endif // UDP_TRANSPORT_H
