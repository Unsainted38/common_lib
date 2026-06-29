#include "um2_13_device.h"
#include "cmd/um2_13_commnad.h"

UM2_13Device::UM2_13Device(SerialCircularRequester *requester, QObject *parent)
    : QObject(parent),
    m_requester(requester)
{
    m_timer = new QTimer(this);
    m_timer->start(1000);

    RStatus = new UM2_13Commnad(0x0000, 24, ValueType::QBYTEARRAY, CommandType::READ);
    RErrors = new UM2_13Commnad(0x0001, 8, ValueType::QBYTEARRAY, CommandType::READ);
    RL1Attenuation = new UM2_13Commnad(0x0020, 4, ValueType::QUINT16, CommandType::READ);
    RL3Attenuation = new UM2_13Commnad(0x0021, 4, ValueType::QUINT16, CommandType::READ);
    RL5Attenuation = new UM2_13Commnad(0x0022, 4, ValueType::QUINT16, CommandType::READ);
    RPathSelect = new UM2_13Commnad(0x0023, 3, ValueType::QUINT8, CommandType::READ);
    WL1Attenuation = new UM2_13Commnad(0x8020, 4, ValueType::QUINT16, CommandType::WRITE);
    WL3Attenuation = new UM2_13Commnad(0x8021, 4, ValueType::QUINT16, CommandType::WRITE);
    WL5Attenuation = new UM2_13Commnad(0x8022, 4, ValueType::QUINT16, CommandType::WRITE);
    WPathSelect = new UM2_13Commnad(0x8023, 3, ValueType::QUINT8, CommandType::WRITE);

    m_requester->addCircularCommand(RStatus);
    m_requester->addCircularCommand(RErrors);
    m_requester->addCircularCommand(RL1Attenuation);
    m_requester->addCircularCommand(RL3Attenuation);
    m_requester->addCircularCommand(RL5Attenuation);
    m_requester->addCircularCommand(RPathSelect);
    m_requester->startRequest();

    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
}

float UM2_13Device::L1Attenuation()
{
    return RL1Attenuation->getValue().toUInt() * 0.5;
}

float UM2_13Device::L3Attenuation()
{
    return RL3Attenuation->getValue().toUInt() * 0.5;
}

float UM2_13Device::L5Attenuation()
{
    return RL5Attenuation->getValue().toUInt() * 0.5;
}

StatusInfo UM2_13Device::status()
{
    return Status;
}

ErrorsInfo UM2_13Device::errors()
{
    return Errors;
}

void UM2_13Device::setL1Attenuation(float attenuation)
{
    quint16 v = ceil(attenuation / 0.5);
    if (v >= 63) v = 63;
    WL1Attenuation->setValue(QVariant::fromValue(v));
}

void UM2_13Device::setL3Attenuation(float attenuation)
{
    quint16 v = ceil(attenuation / 0.5);
    if (v >= 63) v = 63;
    WL3Attenuation->setValue(QVariant::fromValue(v));
    m_requester->addDisposableCommand(WL3Attenuation);
}

void UM2_13Device::setL5Attenuation(float attenuation)
{
    quint16 v = ceil(attenuation / 0.5);
    if (v >= 63) v = 63;
    WL5Attenuation->setValue(QVariant::fromValue(v));
    m_requester->addDisposableCommand(WL5Attenuation);
}

void UM2_13Device::onTimer()
{
    m_statusOnline = false;
}

void UM2_13Device::LastAnswer(QByteArray packet)
{
    m_lastAnswer = packet;
}
