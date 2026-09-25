#include "pvt_device.h"



void PvtDevice::onTimerUpdateData() {
    QVector<quint16> regs = TempHumidityCmd->getValue().value<QVector<quint16>>();
    if (regs.size() < 2) {
        state.online = false;
        return;
    }
    state.temperature = regs[0] / 100.0f;
    state.humidity = regs[1] / 100.0f;
    state.online = true;
}

PvtDevice::PvtDevice(SerialCircularRequester *requester, QString configPath, QString section)
    : AbstractModbusDevice(requester, configPath, section) {
    TempHumidityCmd = new ReadHoldingRegisters(TempReg, 2, protocol);
    addCircularCommand(TempHumidityCmd);
    requester->startRequest();
    m_timer = new QTimer(this);
    m_timer->start(200);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimerUpdateData()));
}
