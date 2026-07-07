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

class TcpTelemetryServer : public QObject
{
    Q_OBJECT
    quint16 port;
    QString name;
    QTcpServer *server;
    QVector<QTcpSocket*> clients;
    QTimer *telemetryTimer;

    ITelemetryPacketSource &m_packetSource;

    void loadConfig(const QString &configPath, const QString &section);

public:
    explicit TcpTelemetryServer(QString configPath, QString section, int period, ITelemetryPacketSource &source, QObject *parent = nullptr);

signals:

private slots:
    void onNewClientConnection();
    void onClientDisconnected();
    void sendTelemetry();
};

#endif // TCP_TELEMETRY_SERVER_H
