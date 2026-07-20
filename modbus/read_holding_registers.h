#ifndef READ_HOLDING_REGISTERS_H
#define READ_HOLDING_REGISTERS_H

#include <QObject>
#include <QVector>
#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>


class ReadHoldingRegisters : public AbstractCommand
{
    QByteArray cachedCommand;
    AbstractModBusProtocol *protocol;
    quint16 registerAddress;
    quint16 registersCount;
    quint8 byteCount;
    QByteArray buffer;
    QByteArray replyHeader;
    QVector<quint16> regs;
    const quint8 cmdID = 0x03;
public:
    explicit ReadHoldingRegisters(quint16 regAddress, quint16 regsCount, AbstractModBusProtocol *protocol, QObject *parent = nullptr);


    // AbstractCommand interface
public:
    const QByteArray &makeCommand() override;

    // AbstractCommand interface
public:
    QVariant getValue() override;
    bool tryParse(const QByteArray &data) override;

public slots:
};

#endif // READ_HOLDING_REGISTERS_H
