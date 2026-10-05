#include "abstract_modbus_device.h"

AbstractModbusDevice::AbstractModbusDevice(std::shared_ptr<SerialCircularRequester> requester, QString configPath, QString section, QObject *parent)
    : QObject(parent),
    requester_(std::move(requester))
{
    protocol_ = ModBusProtocolFactory::getInstance(configPath, section);
    Q_ASSERT(protocol_);
    protocol_->setParent(this);
    device_id_ = protocol_->deviceID();
}

quint8 AbstractModbusDevice::deviceAddress()
{
    return device_id_;
}

void AbstractModbusDevice::addCircularCommand(AbstractCommand *cmd)
{
    requester_->addCircularCommand(cmd);
}

void AbstractModbusDevice::executeCommand(AbstractCommand *cmd)
{
    requester_->addDisposableCommand(cmd);
}

void AbstractModbusDevice::executeNoResponceCommand(AbstractCommand *cmd)
{
    requester_->addNoResponceCommand(cmd);
}
