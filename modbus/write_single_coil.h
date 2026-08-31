#ifndef WRITE_SINGLE_COIL_H
#define WRITE_SINGLE_COIL_H

#include <QObject>
#include <QVector>
#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>


class WriteSingleCoil : public AbstractCommand
{
    Q_OBJECT
    bool coil = false;
    bool cachedCoil = false;
    QByteArray cachedCommand;
    QByteArray cachedPdu;
    AbstractModBusProtocol *protocol;
    quint16 coilAddress;
    QByteArray buffer;
    bool commandStatus = false;
    const quint8 cmdID = 0x05;
public:
    explicit WriteSingleCoil(quint16 coilAddress, AbstractModBusProtocol *protocol, QObject *parent = nullptr);

    // AbstractCommand interface
public:
    const QByteArray &makeCommand() override;
    void setValue(QVariant boolean) override;
    bool isSuccess() override;
    bool tryParse(const QByteArray &data) override;
};

#endif // WRITE_SINGLE_COIL_H
