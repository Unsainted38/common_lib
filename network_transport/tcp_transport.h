#ifndef TCPTRANSPORT_H
#define TCPTRANSPORT_H

#include "abstract_network_transport.h"
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QQueue>
#include <QTimer>
#include <QDateTime>

/**
 * @brief
 *
 */
class TcpTransport : public AbstractNetworkTransport {
    /**
     * @brief
     *
     */
struct PendingPacket {
        quint64 id = 0; /**< TODO: describe */
        QByteArray data; /**< TODO: describe */
        qsizetype acceptedOffset = 0; /**< TODO: describe */
        qsizetype confirmedOffset = 0; /**< TODO: describe */
    };

    QTcpSocket *socket; /**< TODO: describe */
    QHostAddress hostAddress; /**< TODO: describe */
    quint16 port = 7777; /**< TODO: describe */
    QHostAddress listenIp; /**< TODO: describe */
    QString name = "tcp_client"; /**< TODO: describe */
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
explicit TcpTransport(QString configPath, QString section, QObject *parent = nullptr);
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
void setupTransport() override;
    /**
     * @brief
     *
     */
void loadConfig() override;
    /**
     * @brief
     *
     */
void heartbeat() override;
    /**
     * @brief
     *
     * @return bool
     */
bool open() override;

signals:
    //void connectedChanged(bool);
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
     * @param count
     */
void onBytesWritten(qint64 count);
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
public slots:
    //    void onConnected();
    //    void onDisconnected();
    //    void onErrorOccured(QAbstractSocket::SocketError error);
private slots:
    //    void processQueue() override;
    //    void onReadSocket();
    //    void resetTimers();
    //    void scheduleReconnect();
    //    void reconnect();
    //    void onConnected();
    //    void onDisconnected();
    //    void onErrorOccured(QAbstractSocket::SocketError error);
};

#endif // TCPTRANSPORT_H
