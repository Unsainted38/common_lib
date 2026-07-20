#ifndef ABSTRACT_MODBUS_DEVICE_H
#define ABSTRACT_MODBUS_DEVICE_H

#include <QObject>
#include <requesters/serial_circular_requester.h>
#include <utilities/config_helper.h>
#include <modbus/read_holding_registers.h>
#include <modbus/modbus_protocol_factory.h>

class AbstractModbusDevice : public QObject
{
    Q_OBJECT
    quint8 deviceID;
    QString configPath;
    QString section;
    SerialCircularRequester *requester;
    QByteArray buffer;

protected:
    AbstractModBusProtocol *protocol;
    void addCircularCommand(AbstractCommand *cmd);
    void executeCommand(AbstractCommand *cmd);

public:
    explicit AbstractModbusDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent = nullptr);
    quint8 deviceAddress();

signals:

private slots:
};

#endif // ABSTRACT_MODBUS_DEVICE_H
