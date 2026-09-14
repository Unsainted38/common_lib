#include "lir919d_device.h"
#include <utilities/bit_utils.h>

void Lir919dDevice::onTimer()
{
    QVector<quint16> regs = descreteCmd->getValue().value<QVector<quint16>>();
    if (regs.size() < 4) {
        state.online = false;
        return;
    }
    state.online = true;
    state.encoder_descrete = BitUtils::makeFloat(regs[0], regs[1]);
}

Lir919dDevice::Lir919dDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent)
    : AbstractModbusDevice{requester, configPath, section, parent}
{
    descreteCmd = new ReadHoldingRegisters(0, 3, protocol);
    addCircularCommand(descreteCmd);
    requester->startRequest();

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
