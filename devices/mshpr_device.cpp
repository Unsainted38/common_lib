#include "mshpr_device.h"
#include "mshpr_command.h"

MShPRDevice::MShPRDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent)
    : QObject(parent),
      m_configPath(configPath),
      m_section(section),
      m_requester(requester) {
    loadConfig();
    m_parser = new MShPRParser();
    m_timer = new QTimer(this);
    m_timer->start(1000);
    StatusCommand = new MShPRCommand(m_deviceAddr, MShPR_COMMANDS::STATUS_CMD, ValueType::QSTRING, CommandType::READ);
    AttenuationCommand = new MShPRCommand(m_deviceAddr, MShPR_COMMANDS::ATTENUATION_CMD, ValueType::QSTRING, CommandType::WRITE);
    GeterodinCommand = new MShPRCommand(m_deviceAddr, MShPR_COMMANDS::GETERODIN_CMD, ValueType::QSTRING, CommandType::WRITE);
    AddressCommand = new MShPRCommand(m_deviceAddr, MShPR_COMMANDS::ADDRESS_CMD, ValueType::QSTRING, CommandType::WRITE);
    BaudCommand = new MShPRCommand(m_deviceAddr, MShPR_COMMANDS::BAUD_CMD, ValueType::QSTRING, CommandType::WRITE);
    m_requester->addCircularCommand(StatusCommand);
    m_requester->startRequest();

    connect(m_requester, SIGNAL(translateData(QByteArray)), m_parser, SLOT(parseReply(QByteArray)));
    connect(m_parser, SIGNAL(dataReady(QString, QMap)), this, SLOT(processData(QString, QMap)));
    connect(m_parser, SIGNAL(lastAnswer(QByteArray)), this, SLOT(LastAnswer(QByteArray)));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
}

void MShPRDevice::loadConfig() {
    QSettings settings(m_configPath, QSettings::IniFormat);
    settings.beginGroup(m_section);
    m_deviceAddr = settings.value("DeviceAddress", "00").toString();
    settings.endGroup();
}

quint8 MShPRDevice::getAttenuation() {
    return m_attenuation;
}

quint8 MShPRDevice::getGeterodin() {
    return m_geterodin;
}

quint8 MShPRDevice::getStatus() {
    return m_status;
}

quint8 MShPRDevice::getDeviceAddress() {
    return m_deviceAddr.toUInt();
}

QString MShPRDevice::getLastAnswer() {
    return m_lastAnswer;
}

void MShPRDevice::setAttenuation(quint8 value) {
    AttenuationCommand->setS(QString::number(value));
    m_requester->addDisposableCommand(AttenuationCommand);
}

void MShPRDevice::setGeterodin(quint8 value) {
    GeterodinCommand->setS(QString::number(value));
    m_requester->addDisposableCommand(GeterodinCommand);
}

void MShPRDevice::setAddress(quint8 value) {
    m_deviceAddr = QString::number(value);

    AddressCommand->setS(QString::number(value));
    m_requester->addDisposableCommand(AddressCommand);

    if(m_deviceAddr.size() == 1) {
        m_deviceAddr.push_front("0");
    }

    m_requester->removeCommands();
    StatusCommand = new MShPRCommand(m_deviceAddr, MShPR_COMMANDS::STATUS_CMD, ValueType::QSTRING, CommandType::READ);
    AttenuationCommand = new MShPRCommand(m_deviceAddr, MShPR_COMMANDS::ATTENUATION_CMD, ValueType::QSTRING, CommandType::WRITE);
    GeterodinCommand = new MShPRCommand(m_deviceAddr, MShPR_COMMANDS::GETERODIN_CMD, ValueType::QSTRING, CommandType::WRITE);
    AddressCommand = new MShPRCommand(m_deviceAddr, MShPR_COMMANDS::ADDRESS_CMD, ValueType::QSTRING, CommandType::WRITE);
    BaudCommand = new MShPRCommand(m_deviceAddr, MShPR_COMMANDS::BAUD_CMD, ValueType::QSTRING, CommandType::WRITE);
    m_requester->addCircularCommand(StatusCommand);
}

void MShPRDevice::setBaud(MShPRBaud baud) {
    BaudCommand->setS(QString::number(baud));
    m_requester->addDisposableCommand(BaudCommand);
}

void MShPRDevice::setBaud(quint8 baud) {
    BaudCommand->setS(QString::number(baud));
    m_requester->addDisposableCommand(BaudCommand);
}

void MShPRDevice::processData(QString addr, QMap<QString, int> fieldsMap) {
    if(addr != m_deviceAddr) {
        return;
    }

    m_status = fieldsMap[MShPR_COMMANDS::STATUS_CMD];
    m_attenuation = fieldsMap[MShPR_COMMANDS::ATTENUATION_CMD];
    m_geterodin = fieldsMap[MShPR_COMMANDS::GETERODIN_CMD];
    m_timer->start(1000);
}

void MShPRDevice::onTimer() {
    m_status = 0;
}

void MShPRDevice::LastAnswer(QByteArray packet) {
    m_lastAnswer = packet;
}
