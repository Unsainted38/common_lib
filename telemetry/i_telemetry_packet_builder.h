#ifndef I_TELEMETRY_PACKET_BUILDER_H
#define I_TELEMETRY_PACKET_BUILDER_H

#include <QByteArray>

template <typename TData>
class ITelemetryPacketBuilder
{
public:
    virtual ~ITelemetryPacketBuilder() = default;

    virtual QByteArray build(const TData &data) = 0;
};

#endif // I_TELEMETRY_PACKET_BUILDER_H
