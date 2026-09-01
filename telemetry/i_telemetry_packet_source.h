#ifndef I_TELEMETRY_PACKET_SOURCE_H
#define I_TELEMETRY_PACKET_SOURCE_H

#include <QByteArray>

/**
 * @brief
 *
 */
class ITelemetryPacketSource
{
public:
    /**
     * @brief
     *
     */
virtual ~ITelemetryPacketSource() = default;

    /**
     * @brief
     *
     * @return QByteArray
     */
virtual QByteArray makePacket() = 0;

signals:
};

#endif // I_TELEMETRY_PACKET_SOURCE_H
