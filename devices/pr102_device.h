#ifndef PR1_2_DEVICE_H
#define PR1_2_DEVICE_H

#include <modbus_device.h>

class Pr102Device : public ModbusDevice
{
public:
    explicit Pr102Device(std::shared_ptr<SerialCircularRequester> requester, QString config_path, QString section, QObject *parent = nullptr);
    QVector<quint16> getRegisters();
    void setRegisters(QVector<quint16> registers);
private:
    std::unique_ptr<AbstractCommand> getRegisters_, setRegisters_;

    struct Registers {
        quint16 get_registers_{};
        quint16 get_count_{};
        quint16 set_registers_{};
        quint16 set_count_{};
    };
};

#endif // PR1_2_DEVICE_H
