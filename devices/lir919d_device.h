#ifndef LIR919D_DEVICE_H
#define LIR919D_DEVICE_H

#include <modbus/modbus_device.h>
#include <sensors/measurement.h>

struct Lir919dState
{
    bool online = false;
    quint32 encoder_descrete = 0;
    unsigned bits_per_turn = 0;
    unsigned turn_count_bits = 0;
};

class Lir919dDevice : public ModbusDevice
{
    Q_OBJECT

    Measurement<Lir919dState> measurement_;
    AbstractCommand *descrete_cmd_;
    QTimer *m_timer_;
private slots:
    void onTimer();
public:
    explicit Lir919dDevice(std::shared_ptr<SerialCircularRequester> requester, QString configPath, QString section, QObject *parent = nullptr);
    [[nodiscard]]
    Measurement<Lir919dState> measurement() const;
    quint32 descrete() const;
    bool is_online() const;
    unsigned turn_count_bits() const;
    unsigned bits_per_turn() const;
};

#endif // LIR919D_DEVICE_H
