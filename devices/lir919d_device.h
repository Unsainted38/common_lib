#ifndef LIR919D_DEVICE_H
#define LIR919D_DEVICE_H

#include <modbus/abstract_modbus_device.h>



class Lir919dDevice : public AbstractModbusDevice
{
    Q_OBJECT
    struct Lir919dState
    {
        bool online;
        quint32 encoder_descrete;
    } state;
    AbstractCommand *descreteCmd;
    QTimer *m_timer;
private slots:
    void onTimer();
public:
    explicit Lir919dDevice(std::shared_ptr<SerialCircularRequester> requester, QString configPath, QString section, QObject *parent = nullptr);
    quint32 descrete() const;
    bool is_online() const;

};

#endif // LIR919D_DEVICE_H
