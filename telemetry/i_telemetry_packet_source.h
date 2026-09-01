#ifndef I_TELEMETRY_PACKET_SOURCE_H
#define I_TELEMETRY_PACKET_SOURCE_H

#include <QByteArray>

/**
 * @brief Интерфейс источника готовых пакетов телеметрии.
 */
class ITelemetryPacketSource
{
public:
    /**
     * @brief Освобождает ресурсы объекта.
     */
virtual ~ITelemetryPacketSource() = default;

    /**
     * @brief Формирует актуальный пакет телеметрии.
     *
     * @return Сформированный массив байтов.
     */
virtual QByteArray makePacket() = 0;

signals:
};

#endif // I_TELEMETRY_PACKET_SOURCE_H
