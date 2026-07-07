#ifndef I_TELEMETRY_PACKET_SOURCE_H
#define I_TELEMETRY_PACKET_SOURCE_H

#include <QByteArray>

class ITelemetryPacketSource
{
public:
    virtual ~ITelemetryPacketSource() = default;

    virtual QByteArray makePacket() = 0;

signals:
};

#endif // I_TELEMETRY_PACKET_SOURCE_H
