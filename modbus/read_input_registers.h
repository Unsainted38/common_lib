#ifndef READ_INPUT_REGISTERS_H
#define READ_INPUT_REGISTERS_H

#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>


class ReadInputRegisters : public AbstractCommand
{
    Q_OBJECT
    QByteArray cachedCommand;
    AbstractModBusProtocol *protocol;
    quint16 registerAddress;
    quint16 registersCount;
    quint8 byteCount;
    QByteArray buffer;
    QByteArray replyHeader;
    QVector<quint16> regs;
    const quint8 cmdID = 0x04;

public:
    explicit ReadInputRegisters(quint16 regAddress, quint16 regsCount, AbstractModBusProtocol *protocol, QObject *parent = nullptr);

    // AbstractCommand interface
public:
    const QByteArray &makeCommand() override;
    QVariant getValue() override;
    bool tryParse(const QByteArray &data) override;
};

#endif // READ_INPUT_REGISTERS_H
