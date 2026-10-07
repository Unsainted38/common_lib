#ifndef VESPER_ENGINE_DEVICE_H
#define VESPER_ENGINE_DEVICE_H

#include "modbus/modbus_device.h"

struct VesperRegisters {
    static constexpr quint16 Control = 0x0000;
    static constexpr quint16 ReferenceFrequency = 0x0001;
    static constexpr quint16 BroadbandControl = 0x0001;
    static constexpr quint16 ReactionTorque = 0x0028;
};


class VesperEngineDevice : public ModbusDevice
{
    AbstractCommand *startVesperCommand, *stopVesperCommand,
                    *startVesperBroadbandCommand, *stopVesperBroadbandCommand,
                    *reactionTorque;

    void stopVesperBroadband();
    void startVesperBroadband(float freq);
public:
    explicit VesperEngineDevice(std::shared_ptr<SerialCircularRequester> requester, QString configPath, QString section, QObject *parent = nullptr);

    void startVesper(float freq);
    void stopVesper();
    quint16 getReactionTorque();
};

#endif // VESPER_ENGINE_DEVICE_H
