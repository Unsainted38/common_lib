#ifndef DVT_DEVICE_H
#define DVT_DEVICE_H

#include <QObject>
#include <modbus/abstract_modbus_device.h>

struct DvtState {
    quint8 status;
    float pressure;
    float tempetature;
    float humidity;
};

class DvtDevice : public AbstractModbusDevice
{

    AbstractCommand *dvtStateCommand;
    quint8 deviceAddress;
    DvtState dvtInfo;

public:
    explicit DvtDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent = nullptr);
    quint8 status();
    float pressure();
    float temperature();
    float humidity();
signals:
private slots:
    void onTimer();
    void LastAnswer(QByteArray packet);
};

#endif // DVT_DEVICE_H
