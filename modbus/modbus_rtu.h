#ifndef MODBUSRTUCOMMAND_H
#define MODBUSRTUCOMMAND_H

#include <QObject>
#include <modbus/abstractmodbuscommand.h>
#include <cmd/abstract_command.h>

class ModBusRtuCommand : public AbstractModBusCommand
{
    Q_OBJECT
    ModBusCmdTypes modbusType;
public:
    explicit ModBusRtuCommand(quint8 device_address, quint16 reg_address,
                              quint16 regs_count,
                              ModBusCmdTypes modbus_type, ValueType value_type);

signals:

private:
    QByteArray readCoils() override;
    QByteArray readDescreteInputs() override;
    QByteArray readHoldingRegs() override;
    QByteArray readInputRegs() override;
    QByteArray writeSingleCoil() override;
    QByteArray writeSingleReg() override;
    QByteArray writeMultipleCoils() override;
    QByteArray writeMultipleRegisters() override;

    // AbstractCommand interface
public:
    const QByteArray &makeCommand() override;
};

#endif // MODBUSRTUCOMMAND_H
