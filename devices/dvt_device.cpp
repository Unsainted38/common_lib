#include "dvt_device.h"

dvt_device::dvt_device(SerialCircularRequester *requester, QString configPath, QString section)
    : AbstractModbusDevice(requester, configPath, section)
{
    dvtStateCommand = new ModBusRtuCommand(1, 0, 10, ModBusCmdTypes::READ_HOLDING_REGISTERS, ValueType::QBYTEARRAY);
}
