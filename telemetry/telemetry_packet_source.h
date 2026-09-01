#ifndef TELEMETRY_PACKET_SOURCE_H
#define TELEMETRY_PACKET_SOURCE_H

#include "i_telemetry_packet_source.h"
#include "i_telemetry_packet_builder.h"
#include "i_telemetry_provider.h"

template <typename TData>
/**
 * @brief Объединяет поставщика данных и построитель пакетов телеметрии.
 */
class TelemetryPacketSource final : public ITelemetryPacketSource
{
    ITelemetryProvider<TData> &m_provider; /**< Поставщик текущих данных. */
    ITelemetryPacketBuilder<TData> &m_builder; /**< Сериализатор телеметрических данных. */
public:
    /**
     * @brief Объединяет поставщика данных и построитель пакетов телеметрии.
     *
     * @param builder Сериализатор снимка телеметрии.
     * @param provider Поставщик текущего снимка телеметрии.
     */
    TelemetryPacketSource(ITelemetryPacketBuilder<TData> &builder, ITelemetryProvider<TData> &provider)
        :
        m_provider(provider),
        m_builder(builder) {

    }

    /**
     * @brief Формирует актуальный пакет телеметрии.
     *
     * @return Сформированный массив байтов.
     */
QByteArray makePacket() override {
        const TData data = m_provider.currentData();
        return m_builder.build(data);
    };
};

#endif // TELEMETRY_PACKET_SOURCE_H
