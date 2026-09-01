#ifndef TCP_TELEMETRY_SERVER_H
#define TCP_TELEMETRY_SERVER_H

#include <QObject>
#include <QTimer>
#include <QSettings>
#include <QTcpSocket>
#include <QTcpServer>
#include <QVector>
#include <QByteArray>
#include <telemetry/i_telemetry_packet_source.h>

/**
 * @brief
 *
 */
class TcpTelemetryServer : public QObject
{
    Q_OBJECT
    quint16 port; /**< TODO: describe */
    QString name; /**< TODO: describe */
    QTcpServer *server; /**< TODO: describe */
    QVector<QTcpSocket*> clients; /**< TODO: describe */
    QTimer *telemetryTimer; /**< TODO: describe */

    ITelemetryPacketSource &m_packetSource; /**< TODO: describe */

    /**
     * @brief
     *
     * @param configPath
     * @param section
     */
void loadConfig(const QString &configPath, const QString &section);

public:
    /**
     * @brief
     *
     * @param configPath
     * @param section
     * @param period
     * @param source
     * @param parent
     */
explicit TcpTelemetryServer(QString configPath, QString section, int period, ITelemetryPacketSource &source, QObject *parent = nullptr);

signals:

private slots:
    /**
     * @brief
     *
     */
void onNewClientConnection();
    /**
     * @brief
     *
     */
void onClientDisconnected();
    /**
     * @brief
     *
     */
void sendTelemetry();
};

#endif // TCP_TELEMETRY_SERVER_H
