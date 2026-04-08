#include "modbustcpcommand.h"

ModBusTcpCommand::ModBusTcpCommand(quint8 device_address, quint16 reg_address,
                                   quint16 regs_count,
                                   ModBusCmdTypes modbus_type, ValueType value_type)
    :
    AbstractModBusCommand(device_address, reg_address,
                            regs_count,value_type),
    modbusType(modbus_type)
{}

QByteArray ModBusTcpCommand::readCoils()
{

}

QByteArray ModBusTcpCommand::readDescreteInputs()
{

}

QByteArray ModBusTcpCommand::readHoldingRegs()
{

}

QByteArray ModBusTcpCommand::readInputRegs()
{

}

QByteArray ModBusTcpCommand::writeSingleCoil()
{

}

QByteArray ModBusTcpCommand::writeSingleReg()
{

}

QByteArray ModBusTcpCommand::writeMultipleCoils()
{

}

QByteArray ModBusTcpCommand::writeMultipleRegisters()
{

}

const QByteArray &ModBusTcpCommand::makeCommand()
{

}

