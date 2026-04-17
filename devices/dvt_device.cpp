#include "dvt_device.h"

DvtDevice::DvtDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent)
    : AbstractModbusDevice(requester, configPath, section)
{
    dvtStateCommand = new ReadHoldingRegisters(0, 60, protocol);
    addCircularCommand(dvtStateCommand);
}

quint8 DvtDevice::status()
{
    return dvtInfo.status;
}

float DvtDevice::pressure()
{
    return dvtInfo.pressure;
}

float DvtDevice::temperature()
{
    return dvtInfo.tempetature;
}

float DvtDevice::humidity()
{
    return dvtInfo.humidity;
}

void DvtDevice::onTimer()
{
    QVector<quint16> state = dvtStateCommand->getValue().value<QVector<quint16>>();
    dvtInfo.status = state[0];
    dvtInfo.pressure = BitUtils::makeFloat(state[1], state[2]);
    dvtInfo.tempetature = BitUtils::makeFloat(state[5], state[6]);
    dvtInfo.humidity = BitUtils::makeFloat(state[7], state[8]);
}
