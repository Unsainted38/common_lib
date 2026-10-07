#ifndef PVT_DEVICE_H
#define PVT_DEVICE_H
#include <modbus/modbus_device.h>

struct PvtState {
    bool online = false;
    float temperature = 0.0;
    float humidity = 0.0;
};

class PvtDevice : public ModbusDevice {
    Q_OBJECT
    AbstractCommand *TempHumidityCmd;
    const quint16 TempReg = 0x0102;
    PvtState state;
    QTimer *m_timer;
private slots:
    void onTimerUpdateData();
public:
    PvtDevice(std::shared_ptr<SerialCircularRequester> requester, QString configPath, QString section);
    float getTemperature() {
        return state.temperature;
    }
    float getHumidity() {
        return state.humidity;
    }
};
#endif // PVT_DEVICE_H
