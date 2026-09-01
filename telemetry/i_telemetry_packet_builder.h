#ifndef I_TELEMETRY_PACKET_BUILDER_H
#define I_TELEMETRY_PACKET_BUILDER_H

#include <QByteArray>

template <typename TData>
/**
 * @brief
 *
 */
class ITelemetryPacketBuilder
{
public:
    /**
     * @brief
     *
     */
virtual ~ITelemetryPacketBuilder() = default;

    /**
     * @brief
     *
     * @param data
     * @return QByteArray
     */
virtual QByteArray build(const TData &data) = 0;
};

#endif // I_TELEMETRY_PACKET_BUILDER_H
