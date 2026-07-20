#ifndef WRITE_MULTIPLE_COILS_H
#define WRITE_MULTIPLE_COILS_H

#include <QObject>
#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>


class WriteMultipleCoils : public AbstractCommand
{
    Q_OBJECT
    AbstractModBusProtocol *protocol;
    QByteArray cachedCommand;
    quint16 coilAddress;
    quint16 coilsCount;
    QVector<quint8> coils;
    QVector<quint8> cachedCoils;
    quint8 byteCount;
    QByteArray buffer;
    QByteArray replyHeader;
    bool commandStatus;
    const quint8 cmdID = 0x0F;
public:
    explicit WriteMultipleCoils(quint16 coilAddress, quint16 coilsCount, AbstractModBusProtocol *protocol, QObject *parent = nullptr);

    // AbstractCommand interface
public:
    const QByteArray &makeCommand() override;
    void setValue(QVariant v) override;
    bool isSuccess() override;
    bool tryParse(const QByteArray &data) override;
};

#endif // WRITE_MULTIPLE_COILS_H
