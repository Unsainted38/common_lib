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
 * @brief Периодически рассылает телеметрию всем подключённым TCP-клиентам.
 */
class TcpTelemetryServer : public QObject
{
    Q_OBJECT
    quint16 port; /**< Номер локального или удалённого порта. */
    QString name; /**< Диагностическое имя объекта. */
    QTcpServer *server; /**< TCP-сервер входящих подключений. */
    QVector<QTcpSocket*> clients; /**< Активные клиенты рассылки телеметрии. */
    QTimer *telemetryTimer; /**< Таймер периодической рассылки. */

    ITelemetryPacketSource &m_packetSource; /**< Источник готовых пакетов телеметрии. */

    /**
     * @brief Загружает параметры из указанной секции INI-файла.
     *
     * @param configPath Путь к INI-файлу конфигурации.
     * @param section Имя секции с параметрами объекта.
     */
void loadConfig(const QString &configPath, const QString &section);

public:
    /**
     * @brief Периодически рассылает телеметрию всем подключённым TCP-клиентам.
     *
     * @param configPath Путь к INI-файлу конфигурации.
     * @param section Имя секции с параметрами объекта.
     * @param period Период рассылки телеметрии в миллисекундах.
     * @param source Источник готовых пакетов телеметрии.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit TcpTelemetryServer(QString configPath, QString section, int period, ITelemetryPacketSource &source, QObject *parent = nullptr);

signals:

private slots:
    /**
     * @brief Регистрирует новое входящее TCP-соединение.
     */
void onNewClientConnection();
    /**
     * @brief Удаляет отключившегося клиента из списка рассылки.
     */
void onClientDisconnected();
    /**
     * @brief Формирует и отправляет очередной пакет подключённым клиентам.
     */
void sendTelemetry();
};

#endif // TCP_TELEMETRY_SERVER_H
