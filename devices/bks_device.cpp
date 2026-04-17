#include "bks_device.h"
#include "mshpr_command.h"


BksDevice::BksDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent)
    : QObject(parent),
      m_configPath(configPath),
      m_section(section),
      m_requester(requester) {
    m_parser = new MShPRParser();
    m_timer = new QTimer(this);
    m_timer->start(1000);
    StatusCommand = new MShPRCommand(m_deviceAddr, BKS_COMMANDS::STATUS_CMD, CommandType::READ);
    FC1Command = new MShPRCommand(m_deviceAddr, BKS_COMMANDS::FC_LOW_FREQ_CMD, CommandType::WRITE);
    FC2Command = new MShPRCommand(m_deviceAddr, BKS_COMMANDS::FC_HIGH_FREQ_CMD, CommandType::WRITE);
    FX1Command = new MShPRCommand(m_deviceAddr, BKS_COMMANDS::FX_LOW_FREQ_CMD, CommandType::WRITE);
    FX2Command = new MShPRCommand(m_deviceAddr, BKS_COMMANDS::FX_HIGH_FREQ_CMD, CommandType::WRITE);
    AddressCommand = new MShPRCommand(m_deviceAddr, BKS_COMMANDS::STATUS_CMD, CommandType::WRITE);
    BaudCommand = new MShPRCommand(m_deviceAddr, BKS_COMMANDS::STATUS_CMD, CommandType::WRITE);
    m_requester->addCircularCommand(StatusCommand);
    m_requester->startRequest();

    connect(m_requester, SIGNAL(translateData(QByteArray)), m_parser, SLOT(parseReply(QByteArray)));
    connect(m_parser, SIGNAL(dataReady(QString, QMap)), this, SLOT(processData(QString, QMap)));
    connect(m_parser, SIGNAL(lastAnswer(QByteArray)), this, SLOT(LastAnswer(QByteArray)));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
}

void BksDevice::loadConfig() {
    QSettings settings(m_configPath, QSettings::IniFormat);
    settings.beginGroup(m_section);
    m_deviceAddr = settings.value("DeviceAddress", "00").toString();
    settings.endGroup();
}

quint8 BksDevice::getFC1() {
    return m_FC1;
}

quint8 BksDevice::getFC2() {
    return m_FC2;
}

quint8 BksDevice::getFX1() {
    return m_FX1;
}

quint8 BksDevice::getFX2() {
    return m_FX2;
}

quint8 BksDevice::getDeviceAddress() {
    return m_deviceAddr.toUInt();
}

QString BksDevice::getLastAnswer() {
    return m_lastAnswer;
}

bool BksDevice::getStatusOnline() {
    return m_statusOnline;
}

void BksDevice::setFC1(quint8 FC1) {
    FC1Command->setValue(QString::number(FC1));
    m_requester->addDisposableCommand(FC1Command);
}

void BksDevice::setFC2(quint8 FC2) {
    FC2Command->setValue(QString::number(FC2));
    m_requester->addDisposableCommand(FC2Command);
}

void BksDevice::setFX1(quint8 FX1) {
    FX1Command->setValue(QString::number(FX1));
    m_requester->addDisposableCommand(FX1Command);
}

void BksDevice::setFX2(quint8 FX2) {
    FX2Command->setValue(QString::number(FX2));
    m_requester->addDisposableCommand(FX2Command);
}

void BksDevice::setDeviceAddress(quint8 value) {
    AddressCommand->setValue(QString::number(value));
    m_requester->addDisposableCommand(AddressCommand);
}

void BksDevice::setBaud(BksBaud baud) {
    BaudCommand->setValue(QString::number(baud));
    m_requester->addDisposableCommand(BaudCommand);
}

void BksDevice::setBaud(quint8 baud) {
    BaudCommand->setValue(QString::number(baud));
    m_requester->addDisposableCommand(BaudCommand);
}

void BksDevice::LastAnswer(QByteArray packet) {
    m_lastAnswer = packet;
}

void BksDevice::processData(QString addr, QMap<QString, int> fieldsMap) {
    if(addr != m_deviceAddr) {
        return;
    }

    m_statusOnline = true;
    m_FC1 = fieldsMap[BKS_COMMANDS::FC_LOW_FREQ_CMD];
    m_FC2 = fieldsMap[BKS_COMMANDS::FC_HIGH_FREQ_CMD];
    m_FX1 = fieldsMap[BKS_COMMANDS::FX_LOW_FREQ_CMD];
    m_FX2 = fieldsMap[BKS_COMMANDS::FX_HIGH_FREQ_CMD];
    m_timer->start(1000);
}

void BksDevice::onTimer() {
    m_statusOnline = false;
}
