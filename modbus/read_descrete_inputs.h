#ifndef READ_DESCRETE_INPUTS_H
#define READ_DESCRETE_INPUTS_H

#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>

const quint8 cmdID = 0x05;

class ReadDescreteInputs : public AbstractCommand
{
    Q_OBJECT
    AbstractModBusProtocol *protocol;
    QByteArray cachedCommand;
    quint16 inputAddress;
    quint16 inputsCount;
    quint8 byteCount;
    QByteArray buffer;
    QByteArray replyHeader;
    QVector<quint8> inputs;
public:
    explicit ReadDescreteInputs(quint16 inputAddress, quint16 inputsCount, AbstractModBusProtocol *protocol, QObject *parent = nullptr);

    // AbstractCommand interface
public:
    const QByteArray &makeCommand() override;
    QVariant getValue() override;
    bool tryParse(const QByteArray &data) override;
};

#endif // READ_DESCRETE_INPUTS_H
