#include "ubpch_device.h"

UBPChDevice::UBPChDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent)
    : QObject(parent),
      m_requester(requester),
      m_section(section),
      m_configPath(configPath) {
    loadConfig();
    m_timer = new QTimer(this);
    m_timer->start(1000);
    m_parser = new UBPChParser();
    EnableCommand = new UBPChCommand(m_deviceAddr, m_masterAddr, 0x0000, ValueType::QUINT8);
    AttenuationCommand = new UBPChCommand(m_deviceAddr, m_masterAddr, 0x0005, ValueType::QUINT8);
    ErrorStatusCommand = new UBPChCommand(m_deviceAddr, m_masterAddr, 0x0037, ValueType::QUINT8);
    DeviceStatusCommand = new UBPChCommand(m_deviceAddr, m_masterAddr, 0x0014, ValueType::QUINT8);
    OutputPowerCommand = new UBPChCommand(m_deviceAddr, m_masterAddr, 0x0019, ValueType::QUINT16);
    EmpowerCommand = new UBPChCommand(m_deviceAddr, m_masterAddr, 0x001E, ValueType::QUINT16);
    TemperatureCommand = new UBPChCommand(m_deviceAddr, m_masterAddr, 0x0023, ValueType::QINT8);
    VoltageCommand = new UBPChCommand(m_deviceAddr, m_masterAddr, 0x002D, ValueType::QUINT16);
    m_requester->addCommand(EnableCommand);
    m_requester->addCommand(AttenuationCommand);
    m_requester->addCommand(ErrorStatusCommand);
    m_requester->addCommand(DeviceStatusCommand);
    m_requester->addCommand(OutputPowerCommand);
    m_requester->addCommand(EmpowerCommand);
    m_requester->addCommand(TemperatureCommand);
    m_requester->addCommand(VoltageCommand);
    m_requester->startRequest();

    connect(m_requester, SIGNAL(translateData(QByteArray)), m_parser, SLOT(parseReply(QByteArray)));
    connect(m_parser, SIGNAL(dataReady(QByteArray, quint16)), this, SLOT(onStatusOnline()));
    connect(m_parser, SIGNAL(dataReady(QByteArray, quint16)), EnableCommand, SLOT(processData(QByteArray, quint16)));
    connect(m_parser, SIGNAL(dataReady(QByteArray, quint16)), AttenuationCommand, SLOT(processData(QByteArray, quint16)));
    connect(m_parser, SIGNAL(dataReady(QByteArray, quint16)), ErrorStatusCommand, SLOT(processData(QByteArray, quint16)));
    connect(m_parser, SIGNAL(dataReady(QByteArray, quint16)), DeviceStatusCommand, SLOT(processData(QByteArray, quint16)));
    connect(m_parser, SIGNAL(dataReady(QByteArray, quint16)), OutputPowerCommand, SLOT(processData(QByteArray, quint16)));
    connect(m_parser, SIGNAL(dataReady(QByteArray, quint16)), EmpowerCommand, SLOT(processData(QByteArray, quint16)));
    connect(m_parser, SIGNAL(dataReady(QByteArray, quint16)), TemperatureCommand, SLOT(processData(QByteArray, quint16)));
    connect(m_parser, SIGNAL(dataReady(QByteArray, quint16)), VoltageCommand, SLOT(processData(QByteArray, quint16)));
    connect(m_parser, SIGNAL(lastAnswer(QByteArray)), this, SLOT(onLastAnswer(QByteArray)));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
}

void UBPChDevice::loadConfig() {
    QSettings settings(m_configPath, QSettings::IniFormat);
    settings.beginGroup(m_section);
    m_deviceAddr = settings.value("DeviceAddr", 0x16).toUInt();
    m_masterAddr = settings.value("MasterAddr", 0x01).toUInt();
    settings.endGroup();
}

void UBPChDevice::setEnabled(quint8 value) {
    EnableCommand->setV<quint8>(value);
    m_requester->addExtraCommand(EnableCommand);
}

quint8 UBPChDevice::getEnabled() {
    return EnableCommand->getV<quint8>();
}

void UBPChDevice::setAttenuation(quint8 value) {
    AttenuationCommand->setV<quint8>(value);
    m_requester->addExtraCommand(AttenuationCommand);
}

quint32 UBPChDevice::getAttenuation() {
    return AttenuationCommand->getV<quint32>();
}

quint8 UBPChDevice::getErrorStatus() {
    return ErrorStatusCommand->getV<quint8>();
}

quint8 UBPChDevice::getDeviceStatus() {
    return DeviceStatusCommand->getV<quint8>();
}

float UBPChDevice::getOutputPower() {
    return OutputPowerCommand->getV<quint32>() / 10.0;
}

float UBPChDevice::getEmpower() {
    return EmpowerCommand->getV<quint32>() / 10.0;
}

qint32 UBPChDevice::getTemperature() {
    return TemperatureCommand->getV<qint32>();
}

float UBPChDevice::getVoltage() {
    float res = VoltageCommand->getV<quint16>() / 10.0;
    return res;
}

bool UBPChDevice::getStatusOnline() {
    return m_statusOnline;
}

void UBPChDevice::onLastAnswer(QByteArray packet) {
    m_lastAnswer = packet;
}

QByteArray UBPChDevice::getLastAnswer() {
    return m_lastAnswer;
}

void UBPChDevice::onTimer() {
    m_statusOnline = false;
}

void UBPChDevice::onStatusOnline() {
    m_statusOnline = true;
    m_timer->start(1000);
}
