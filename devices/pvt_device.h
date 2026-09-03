#ifndef PVT_DEVICE_H
#define PVT_DEVICE_H
#include <modbus/abstract_modbus_device.h>

struct PvtState {
    float temperature = 0.0;
    float humidity = 0.0;
};

class PvtDevice : public AbstractModbusDevice {
    Q_OBJECT
    AbstractCommand *TempHumidityCmd;
    const quint16 TempReg = 0x0102;
    PvtState info;
    QTimer *m_timer;
private slots:
    void onTimerUpdateData();
public:
    PvtDevice(SerialCircularRequester* requester, QString configPath, QString section);
    float getTemperature() {
        return info.temperature;
    }
    float getHumidity() {
        return info.humidity;
    }
};
#endif // PVT_DEVICE_H
