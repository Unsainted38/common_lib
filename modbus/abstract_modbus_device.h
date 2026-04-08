#ifndef ABSTRACT_MODBUS_DEVICE_H
#define ABSTRACT_MODBUS_DEVICE_H

#include <QObject>
#include <requesters/serial_circular_requester.h>
#include <utilities/config_helper.h>
#include <modbus/modbus_parser.h>

class AbstractModbusDevice : public QObject
{
    Q_OBJECT
    quint8 address;
    QString configPath;
    QString section;
    SerialCircularRequester *requester;
    ModBusParser *parser;
    QByteArray buffer;

public:
    explicit AbstractModbusDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent = nullptr);
    quint8 deviceAddress();

signals:

private slots:
    virtual void processData();
};

#endif // ABSTRACT_MODBUS_DEVICE_H
