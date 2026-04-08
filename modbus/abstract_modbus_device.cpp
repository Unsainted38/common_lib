#include "abstract_modbus_device.h"

AbstractModbusDevice::AbstractModbusDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent)
    : QObject(parent),
    requester(requester)
{
    parser = new ModBusParser();
    address = ConfigHelper::loadModBusDeviceAddress(configPath, section);

    connect()
}

quint8 AbstractModbusDevice::deviceAddress()
{
    return address;
}
