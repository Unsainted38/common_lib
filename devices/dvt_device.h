#ifndef DVT_DEVICE_H
#define DVT_DEVICE_H

#include <QObject>
#include <modbus/modbusrtucommand.h>
#include <requesters/serial_circular_requester.h>
#include <modbus/abstract_modbus_device.h>
#include <modbus/modbus_parser.h>

struct DvtState {
    quint8 status;
    float pressure;
    float tempetature;
    float humidity;
};

class dvt_device : public AbstractModbusDevice
{

    AbstractCommand *dvtStateCommand;
    quint8 deviceAddress;
    DvtState dvtInfo;

public:
    explicit dvt_device(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent = nullptr);

signals:
private slots:
    void processData();
    void onTimer();
    void LastAnswer(QByteArray packet);
};

#endif // DVT_DEVICE_H
