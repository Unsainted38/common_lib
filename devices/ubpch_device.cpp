#include "ubpch_device.h"
#include "cmd/ubpch_command.h"

UBPChDevice::UBPChDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent)
    : QObject(parent),
      m_configPath(configPath),
      m_section(section),
      m_requester(requester) {
    loadConfig();
    m_timer = new QTimer(this);
    m_timer->start(1000);
    m_parser = new UBPChParser();
    EnableCommand = new UBPChCommand(m_deviceAddr, m_masterAddr, 0x0000, ValueType::QUINT8, CommandType::READ);
    AttenuationCommand = new UBPChCommand(m_deviceAddr, m_masterAddr, 0x0005, ValueType::QUINT8, CommandType::READ);
    ErrorStatusCommand = new UBPChCommand(m_deviceAddr, m_masterAddr, 0x0037, ValueType::QUINT8, CommandType::READ);
    DeviceStatusCommand = new UBPChCommand(m_deviceAddr, m_masterAddr, 0x0014, ValueType::QUINT8, CommandType::READ);
    OutputPowerCommand = new UBPChCommand(m_deviceAddr, m_masterAddr, 0x0019, ValueType::QUINT16, CommandType::READ);
    EmpowerCommand = new UBPChCommand(m_deviceAddr, m_masterAddr, 0x001E, ValueType::QUINT16, CommandType::READ);
    TemperatureCommand = new UBPChCommand(m_deviceAddr, m_masterAddr, 0x0023, ValueType::QINT8, CommandType::READ);
    VoltageCommand = new UBPChCommand(m_deviceAddr, m_masterAddr, 0x002D, ValueType::QUINT16, CommandType::READ);
    m_requester->addCircularCommand(EnableCommand);
    m_requester->addCircularCommand(AttenuationCommand);
    m_requester->addCircularCommand(ErrorStatusCommand);
    m_requester->addCircularCommand(DeviceStatusCommand);
    m_requester->addCircularCommand(OutputPowerCommand);
    m_requester->addCircularCommand(EmpowerCommand);
    m_requester->addCircularCommand(TemperatureCommand);
    m_requester->addCircularCommand(VoltageCommand);
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
    EnableCommand->setValue(value);
    m_requester->addDisposableCommand(EnableCommand);
}

quint8 UBPChDevice::getEnabled() {
    return EnableCommand->getValue().toUInt();
}

void UBPChDevice::setAttenuation(quint8 value) {
    AttenuationCommand->setValue(value);
    m_requester->addDisposableCommand(AttenuationCommand);
}

quint32 UBPChDevice::getAttenuation() {
    return AttenuationCommand->getValue().toUInt();
}

quint8 UBPChDevice::getErrorStatus() {
    return ErrorStatusCommand->getValue().toUInt();
}

quint8 UBPChDevice::getDeviceStatus() {
    return DeviceStatusCommand->getValue().toUInt();
}

float UBPChDevice::getOutputPower() {
    return OutputPowerCommand->getValue().toFloat() / 10.0;
}

float UBPChDevice::getEmpower() {
    return EmpowerCommand->getValue().toFloat() / 10.0;
}

qint32 UBPChDevice::getTemperature() {
    return TemperatureCommand->getValue().toInt();
}

float UBPChDevice::getVoltage() {
    float res = VoltageCommand->getValue().toFloat() / 10.0;
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
