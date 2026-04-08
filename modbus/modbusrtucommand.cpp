#include "modbusrtucommand.h"

ModBusRtuCommand::ModBusRtuCommand(quint8 device_address, quint16 reg_address,
                                   quint16 regs_count,
                                   ModBusCmdTypes modbus_type, ValueType value_type)
    :
    AbstractModBusCommand(device_address, reg_address,
                            regs_count, value_type),
    modbusType(modbus_type)
{}

QByteArray ModBusRtuCommand::readCoils()
{
    QByteArray res;
    QDataStream out(&res, QIODevice::WriteOnly);
    out.setByteOrder(byteOrder);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out << device_address
        << *ModBusCmdTypes::READ_COILS
        << reg_address
        << regs_count
        << GetCrc16(res);
    return res;
}

QByteArray ModBusRtuCommand::readDescreteInputs()
{
    QByteArray res;
    QDataStream out(&res, QIODevice::WriteOnly);
    out.setByteOrder(byteOrder);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out << device_address
        << *ModBusCmdTypes::READ_DESCRETE_INPUTS
        << reg_address
        << regs_count
        << GetCrc16(res);
    return res;
}

QByteArray ModBusRtuCommand::readHoldingRegs()
{
    QByteArray res;
    QDataStream out(&res, QIODevice::WriteOnly);
    out.setByteOrder(byteOrder);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out << device_address
        << *ModBusCmdTypes::READ_HOLDING_REGISTERS
        << reg_address
        << regs_count
        << GetCrc16(res);
    return res;
}

QByteArray ModBusRtuCommand::readInputRegs()
{
    QByteArray res;
    QDataStream out(&res, QIODevice::WriteOnly);
    out.setByteOrder(byteOrder);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out << device_address
        << *ModBusCmdTypes::READ_INPUT_REGISTERS
        << reg_address
        << regs_count
        << GetCrc16(res);
    return res;
}

QByteArray ModBusRtuCommand::writeSingleCoil()
{
    quint16 value;
    m_valBool ? value = 0xFF00 : value = 0x0000;
    QByteArray res;
    QDataStream out(&res, QIODevice::WriteOnly);
    out.setByteOrder(byteOrder);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out << device_address
        << *ModBusCmdTypes::WRITE_SINGLE_COIL
        << reg_address
        << value
        << GetCrc16(res);
    return res;
}

QByteArray ModBusRtuCommand::writeSingleReg()
{
    QByteArray res;
    QDataStream out(&res, QIODevice::WriteOnly);
    out.setByteOrder(byteOrder);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out << device_address
        << *ModBusCmdTypes::WRITE_SINGLE_REGISTER
        << reg_address
        << m_valu16
        << GetCrc16(res);
    return res;
}

QByteArray ModBusRtuCommand::writeMultipleCoils()
{
    quint8 byte_count = m_valListQuint8.size() * sizeof(quint8);
    QByteArray res;
    QDataStream out(&res, QIODevice::WriteOnly);
    out.setByteOrder(byteOrder);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out << device_address
        << *ModBusCmdTypes::WRITE_MULTIPLE_COILS
        << reg_address
        << regs_count
        << byte_count;
    for (const quint8 item : m_valListQuint8) {
        out << item;
    }
        out << GetCrc16(res);
    return res;
}

QByteArray ModBusRtuCommand::writeMultipleRegisters()
{
    quint8 byte_count = m_valListQuint16.size() * sizeof(quint16);
    QByteArray res;
    QDataStream out(&res, QIODevice::WriteOnly);
    out.setByteOrder(byteOrder);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out << device_address
        << *ModBusCmdTypes::WRITE_MULTIPLE_REGISTERS
        << reg_address
        << regs_count
        << byte_count;
    for (const quint16 item : m_valListQuint16) {
        out << item;
    }
        out << GetCrc16(res);
    return res;
}

const QByteArray &ModBusRtuCommand::makeCommand()
{
    if (!cachedCommand.isEmpty()) {
        return cachedCommand;
    }
    switch (modbusType) {
        case ModBusCmdTypes::READ_COILS:
            cachedCommand = readCoils();
            break;
        case ModBusCmdTypes::READ_DESCRETE_INPUTS:
            cachedCommand = readDescreteInputs();
            break;
        case ModBusCmdTypes::READ_HOLDING_REGISTERS:
            cachedCommand = readHoldingRegs();
            break;
        case ModBusCmdTypes::READ_INPUT_REGISTERS:
            cachedCommand = readInputRegs();
            break;
        case ModBusCmdTypes::WRITE_MULTIPLE_COILS:
            cachedCommand = writeMultipleCoils();
            break;
        case ModBusCmdTypes::WRITE_SINGLE_COIL:
            cachedCommand = writeSingleCoil();
            break;
        case ModBusCmdTypes::WRITE_MULTIPLE_REGISTERS:
            cachedCommand = writeMultipleRegisters();
            break;
        case ModBusCmdTypes::WRITE_SINGLE_REGISTER:
            cachedCommand = writeSingleReg();
            break;
    }
        return cachedCommand;
}


