#include "abstractmodbuscommand.h"

AbstractModBusCommand::AbstractModBusCommand(quint8 device_address, quint16 reg_address, quint16 regs_count, ValueType value_type, QDataStream::ByteOrder byte_order)
    :
    AbstractCommand(value_type, CommandType::MODBUS),
    device_address(device_address),
    reg_address(reg_address),
    regs_count(regs_count),
    byteOrder(byte_order)
{}
