#ifndef MODBUSTCPCOMMAND_H
#define MODBUSTCPCOMMAND_H

#include <QObject>
#include <modbus/abstractmodbuscommand.h>

class ModBusTcpCommand : public AbstractModBusCommand
{
    Q_OBJECT
    ModBusCmdTypes modbusType;

public:
    explicit ModBusTcpCommand(quint8 device_address, quint16 reg_address,
                              quint16 regs_count,
                              ModBusCmdTypes modbus_type, ValueType value_type);
    QByteArray readCoils() override;
    QByteArray readDescreteInputs() override;
    QByteArray readHoldingRegs() override;
    QByteArray readInputRegs() override;
    QByteArray writeSingleCoil() override;
    QByteArray writeSingleReg() override;
    QByteArray writeMultipleCoils() override;
    QByteArray writeMultipleRegisters() override;
signals:

    // AbstractCommand interface
public:
    const QByteArray &makeCommand() override;

};

#endif // MODBUSTCPCOMMAND_H
