#ifndef WRITE_MULTIPLE_REGISTERS_H
#define WRITE_MULTIPLE_REGISTERS_H

#include <QObject>
#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>


class WriteMultipleRegisters : public AbstractCommand
{
    AbstractModBusProtocol *protocol;
    QByteArray cachedCommand;
    quint16 registerAddress;
    quint16 registersCount;
    quint8 byteCount;
    QByteArray buffer;
    QByteArray replyHeader;
    QVector<quint16> regs;
    QVector<quint16> cachedRegs;
    bool commandStatus;
    const quint8 cmdID = 0x10;
public:
    explicit WriteMultipleRegisters(quint16 regAddress, quint16 regsCount, AbstractModBusProtocol *protocol, QObject *parent = nullptr);

    // AbstractCommand interface
public:
    const QByteArray &makeCommand() override;

    // AbstractCommand interface
public:
    void setValue(QVariant v) override;
    bool isSuccess() override;
    bool tryParse(const QByteArray &data) override;
};

#endif // WRITE_MULTIPLE_REGISTERS_H
