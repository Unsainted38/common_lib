#ifndef READ_COILS_H
#define READ_COILS_H

#include <QObject>
#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>


class ReadCoils : public AbstractCommand
{
    Q_OBJECT
    QByteArray cachedCommand;
    AbstractModBusProtocol *protocol;
    quint16 coilAddress;
    quint16 coilsCount;
    quint8 byteCount;
    QByteArray buffer;
    QByteArray replyHeader;
    QVector<quint8> coils;
    const quint8 cmdID = 0x01;
public:
    explicit ReadCoils(quint16 coilAddress, quint16 coilsCount, AbstractModBusProtocol *protocol, QObject *parent = nullptr);

    // AbstractCommand interface
public:
    const QByteArray &makeCommand() override;

    // AbstractCommand interface
public:
    QVariant getValue() override;
    bool tryParse(const QByteArray &data) override;


};

#endif // READ_COILS_H
