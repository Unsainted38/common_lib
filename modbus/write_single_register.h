#ifndef WRITE_SINGLE_REGISTER_H
#define WRITE_SINGLE_REGISTER_H

#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>



class WriteSingleRegister : public AbstractCommand
{
    Q_OBJECT
    AbstractModBusProtocol *protocol;
    QByteArray cachedCommand;
    quint16 registerAddress;
    quint16 registersCount;
    quint16 reg;
    quint16 cachedReg;
    quint8 byteCount;
    QByteArray buffer;
    QByteArray replyHeader;
    bool commandStatus;
    const quint8 cmdID = 0x06;
public:
    explicit WriteSingleRegister(quint16 regAddress, AbstractModBusProtocol *protocol, QObject *parent = nullptr);

    // AbstractCommand interface
public:
    const QByteArray &makeCommand() override;
    void setValue(QVariant v) override;
    bool isSuccess() override;
    bool tryParse(const QByteArray &data) override;
};

#endif // WRITE_SINGLE_REGISTER_H
