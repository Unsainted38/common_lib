#include "lir919d_device.h"
#include <utilities/bit_utils.h>
#include "modbus/read_input_registers.h"

void Lir919dDevice::onTimer()
{
    QVector<quint16> regs = descreteCmd->getValue().value<QVector<quint16>>();
    if (regs.size() < 3) {
        state.online = false;
        return;
    }
    state.online = true;
    state.encoder_descrete = BitUtils::makeQuint32(regs[1], regs[2]);
}

Lir919dDevice::Lir919dDevice(std::shared_ptr<SerialCircularRequester> requester, QString configPath, QString section, QObject *parent)
    : AbstractModbusDevice{std::move(requester), configPath, section, parent}
{
    descreteCmd = new ReadInputRegisters(0x10, 3, protocol_);
    addCircularCommand(descreteCmd);

    m_timer = new QTimer(this);
    m_timer->start(100);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
}

quint32 Lir919dDevice::descrete() const
{
    return state.encoder_descrete;
}

bool Lir919dDevice::is_online() const
{
    return state.online;
}
