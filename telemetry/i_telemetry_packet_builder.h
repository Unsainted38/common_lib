#ifndef I_TELEMETRY_PACKET_BUILDER_H
#define I_TELEMETRY_PACKET_BUILDER_H

#include <QByteArray>

template <typename TData>
/**
 * @brief Интерфейс сериализации снимка телеметрии в сетевой пакет.
 */
class ITelemetryPacketBuilder
{
public:
    /**
     * @brief Освобождает ресурсы объекта.
     */
virtual ~ITelemetryPacketBuilder() = default;

    /**
     * @brief Сериализует переданный снимок данных.
     *
     * @param data Входные данные или полезная нагрузка ответа.
     * @return Сформированный массив байтов.
     */
virtual QByteArray build(const TData &data) = 0;
};

#endif // I_TELEMETRY_PACKET_BUILDER_H
