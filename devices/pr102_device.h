#ifndef PR1_2_DEVICE_H
#define PR1_2_DEVICE_H

#include <modbus_device.h>
#include <sensors/measurement.h>

struct Registers {
    quint16 get_registers_{};
    quint16 get_count_{};
    quint16 set_registers_{};
    quint16 set_count_{};
};

struct Pr102State {
    bool online = false;
    QVector<quint16> registers_data{};
};

class Pr102Device : public ModbusDevice
{
public:
    explicit Pr102Device(std::shared_ptr<SerialCircularRequester> requester, QString config_path, QString section, QObject *parent = nullptr);
    QVector<quint16> getRegisters();
    void setRegisters(QVector<quint16> registers);
    bool is_online() const;
    Measurement<Pr102State> measurement() const;
private:
    std::unique_ptr<AbstractCommand> getRegistersCmd_, setRegistersCmd_;
    Measurement<Pr102State> measurement_;
    void update_measurement();
    void invalidate_measurement();

    std::unique_ptr<QTimer> timer_;
};

#endif // PR1_2_DEVICE_H
