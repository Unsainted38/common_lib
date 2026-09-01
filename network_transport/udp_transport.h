#ifndef UDP_TRANSPORT_H
#define UDP_TRANSPORT_H

#include <QObject>
#include "abstract_network_transport.h"
#include <QUdpSocket>
#include <QQueue>
#include <QTimer>
#include <QDateTime>


/**
 * @brief
 *
 */
class UdpTransport : public AbstractNetworkTransport
{
    Q_OBJECT
    /**
     * @brief
     *
     */
struct PendingPacket {
        quint64 id = 0; /**< TODO: describe */
        QByteArray data; /**< TODO: describe */
    };

    QUdpSocket *socket; /**< TODO: describe */
    QHostAddress hostAddress; /**< TODO: describe */
    quint16 port = 7777; /**< TODO: describe */
    QHostAddress listenIp; /**< TODO: describe */
    QString name = "udp_client"; /**< TODO: describe */
    QMutex mutex; /**< TODO: describe */
    QQueue<PendingPacket> queue; /**< TODO: describe */
    quint64 nextPacketId = 1; /**< TODO: describe */
    QTimer *heartbeatTimer; /**< TODO: describe */
    QTimer *reconnectTimer; /**< TODO: describe */
    bool connectedState = false; /**< TODO: describe */
    bool reconnectEnabled = true; /**< TODO: describe */
    QDateTime lastActivity; /**< TODO: describe */
    QByteArray heartbeatPacket; /**< TODO: describe */

public:
    /**
     * @brief
     *
     * @param configPath
     * @param section
     * @param parent
     */
explicit UdpTransport(QString configPath, QString section, QObject *parent = nullptr);

    // AbstractNetworkTransport interface
public:
    /**
     * @brief
     *
     */
void loadConfig() override;
    /**
     * @brief
     *
     */
void setupTransport() override;
    /**
     * @brief
     *
     * @return bool
     */
bool open() override;
    /**
     * @brief
     *
     * @param packet
     * @return bool
     */
bool write(const QByteArray &packet) override;
    /**
     * @brief
     *
     * @param packet
     * @return quint64
     */
quint64 writeTracked(const QByteArray &packet) override;
    /**
     * @brief
     *
     * @return bool
     */
bool close() override;
    /**
     * @brief
     *
     */
void heartbeat() override;

private:
    /**
     * @brief
     *
     */
void onConnected();
    /**
     * @brief
     *
     */
void onDisconnected();
    /**
     * @brief
     *
     * @param error
     */
void onErrorOccured(QAbstractSocket::SocketError error);
    /**
     * @brief
     *
     */
void processQueue() override;
    /**
     * @brief
     *
     */
void onReadSocket();
    /**
     * @brief
     *
     */
void resetTimers();
    /**
     * @brief
     *
     */
void scheduleReconnect();
    /**
     * @brief
     *
     */
void reconnect();
};

#endif // UDP_TRANSPORT_H
