#include "abstract_modbus_device.h"

AbstractModbusDevice::AbstractModbusDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent)
    : QObject(parent),
    requester(requester)
{
    protocol = ModBusProtocolFactory::getInstance(configPath, section);
    deviceID = protocol->deviceID();
}

quint8 AbstractModbusDevice::deviceAddress()
{
    return deviceID;
}

void AbstractModbusDevice::addCircularCommand(AbstractCommand *cmd)
{
    requester->addCircularCommand(cmd);
}

void AbstractModbusDevice::executeCommand(AbstractCommand *cmd)
{
    requester->addDisposableCommand(cmd);
}
