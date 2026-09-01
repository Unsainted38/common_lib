#ifndef TELEMETRY_PACKET_SOURCE_H
#define TELEMETRY_PACKET_SOURCE_H

#include "i_telemetry_packet_source.h"
#include "i_telemetry_packet_builder.h"
#include "i_telemetry_provider.h"

template <typename TData>
/**
 * @brief
 *
 */
class TelemetryPacketSource final : public ITelemetryPacketSource
{
    ITelemetryProvider<TData> &m_provider; /**< TODO: describe */
    ITelemetryPacketBuilder<TData> &m_builder; /**< TODO: describe */
public:
    /**
     * @brief
     *
     * @param builder
     * @param provider
     */
    TelemetryPacketSource(ITelemetryPacketBuilder<TData> &builder, ITelemetryProvider<TData> &provider)
        :
        m_provider(provider),
        m_builder(builder) {

    }

    /**
     * @brief
     *
     * @return QByteArray
     */
QByteArray makePacket() override {
        const TData data = m_provider.currentData();
        return m_builder.build(data);
    };
};

#endif // TELEMETRY_PACKET_SOURCE_H
