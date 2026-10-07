#include "vesper_engine_device.h"
#include "modbus/write_multiple_registers.h"
#include "modbus/read_holding_registers.h"

VesperEngineDevice::VesperEngineDevice(std::shared_ptr<SerialCircularRequester> requester, QString configPath, QString section, QObject *parent)
    : ModbusDevice{std::move(requester), configPath, section, parent},
    startVesperCommand(new WriteMultipleRegisters(VesperRegisters::Control, 2, protocol_)),
    stopVesperCommand(new WriteMultipleRegisters(VesperRegisters::Control, 2, protocol_)),
    startVesperBroadbandCommand(new WriteMultipleRegisters(VesperRegisters::BroadbandControl, 2, protocol_)),
    stopVesperBroadbandCommand(new WriteMultipleRegisters(VesperRegisters::BroadbandControl, 2, protocol_)),
    reactionTorque(new ReadHoldingRegisters(VesperRegisters::ReactionTorque, 1, protocol_))
{
    if (protocol_->deviceID() != 0) {
        //requester->addCircularCommand(reactionTorque);
    }
}

void VesperEngineDevice::startVesperBroadband(float freq)
{
    quint16 direction;
    if (freq == 0) {
        stopVesperBroadband();
        return;
    }
    else if (freq > 0) {
        direction = 1;
    } else {
        direction = 3;
    }
    quint16 freq_descrete = static_cast<quint16>(std::abs(freq)) * 600;
    QVector<quint16> regs({direction,freq_descrete});
    startVesperBroadbandCommand->setValue(QVariant::fromValue(regs));
    executeNoResponceCommand(startVesperBroadbandCommand);
}

void VesperEngineDevice::startVesper(float freq)
{
    if (deviceAddress() == 0x00) {
        startVesperBroadband(freq);
        return;
    }
    quint16 direction;
    if (freq == 0) {
        stopVesper();
        return;
    }
    else if (freq > 0) {
        direction = 1;
    } else {
        direction = 2;
    }
    quint16 freq_descrete = static_cast<quint16>(freq) * 100;
    QVector<quint16> regs({direction,freq_descrete});
    startVesperCommand->setValue(QVariant::fromValue(regs));
    executeCommand(startVesperCommand);
}

void VesperEngineDevice::stopVesperBroadband()
{
    QVector<quint16> regs({0,0});
    stopVesperBroadbandCommand->setValue(QVariant::fromValue(regs));
    executeNoResponceCommand(stopVesperBroadbandCommand);
}

void VesperEngineDevice::stopVesper()
{
    if (deviceAddress() == 0x00) {
        stopVesperBroadband();
        return;
    }
    QVector<quint16> regs({0,0});
    stopVesperCommand->setValue(QVariant::fromValue(regs));
    executeCommand(stopVesperCommand);
}

quint16 VesperEngineDevice::getReactionTorque()
{
    return reactionTorque->getValue().value<quint16>();
}
