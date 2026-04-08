#ifndef ABSTRACTMODBUSCOMMAND_H
#define ABSTRACTMODBUSCOMMAND_H

#include <QObject>
#include <algorithm/crc16.h>
#include <cmd/abstract_command.h>
#include <modbus/ModBusCmdTypes.h>

class AbstractModBusCommand : public AbstractCommand
{
    Q_OBJECT


public:
    explicit AbstractModBusCommand(quint8 device_address, quint16 reg_address, quint16 regs_count, ValueType value_type,
                                   QDataStream::ByteOrder byte_order  = QDataStream::BigEndian);

signals:

protected:
    quint8 device_address;
    quint16 reg_address;
    quint16 regs_count;
    QDataStream::ByteOrder byteOrder;
    QByteArray cachedCommand;

    virtual QByteArray readCoils() = 0;
    virtual QByteArray readDescreteInputs() = 0;
    virtual QByteArray readHoldingRegs() = 0;
    virtual QByteArray readInputRegs() = 0;
    virtual QByteArray writeSingleCoil() = 0;
    virtual QByteArray writeSingleReg() = 0;
    virtual QByteArray writeMultipleCoils() = 0;
    virtual QByteArray writeMultipleRegisters() = 0;

    // AbstractCommand interface
public:
    const QByteArray &makeCommand() = 0;
};

#endif // ABSTRACTMODBUSCOMMAND_H
