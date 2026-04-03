#include "compass_lcc5000_device.h"

CompassLCC5000Device::CompassLCC5000Device(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent)
    : QObject(parent),
      m_requester(requester),
      m_configPath(configPath),
      m_section(section) {
    loadConfig();
    m_parser = new CompassLCC5000Parser(m_deviceAddr);
    m_timer = new QTimer(this);
    m_timer->start(1000);
    AllAnglesRequest = new CompassLCC5000Command(m_deviceAddr, CompassCommands::ALLANGLE, 0x04, ValueType::DOUBLE);
    PitchRequest = new CompassLCC5000Command(m_deviceAddr, CompassCommands::PITCH, 0x04, ValueType::DOUBLE);
    RollRequest = new CompassLCC5000Command(m_deviceAddr, CompassCommands::ROLL, 0x04, ValueType::DOUBLE);
    HeadingRequest = new CompassLCC5000Command(m_deviceAddr, CompassCommands::HEADING, 0x04, ValueType::DOUBLE);
    MagneticDeclinationCommand = new CompassLCC5000Command(m_deviceAddr, CompassCommands::SETMAGNETICDECLINATION, 0x06, ValueType::DOUBLE);
    MagneticDeclinationRequest = new CompassLCC5000Command(m_deviceAddr, CompassCommands::MAGNETICDECLINATION, 0x04, ValueType::DOUBLE);
    BaudRateCommand = new CompassLCC5000Command(m_deviceAddr, CompassCommands::BAUDRATE, 0x05, ValueType::QUINT8);
    ModuleAddressCommand = new CompassLCC5000Command(m_deviceAddr, CompassCommands::SETMODULEADDRESS, 0x05, ValueType::QUINT8);
    CurrentAddressRequest = new CompassLCC5000Command(m_deviceAddr, CompassCommands::MODULEADDRESS, 0x04, ValueType::QUINT8);
    OutputAngleModeCommand = new CompassLCC5000Command(m_deviceAddr, CompassCommands::SETOUTPUTANGLEMODE, 0x05, ValueType::QUINT8);
    SaveSettingsCommand = new CompassLCC5000Command(m_deviceAddr, CompassCommands::SAVESETTINGS, 0x04, ValueType::QUINT8);
    SwitchCalibrationOutpuRequest = new CompassLCC5000Command(m_deviceAddr, CompassCommands::SWITCHCALIBRATIONOUTPUT, 0x04, ValueType::QUINT8);

    m_requester->addCommand(AllAnglesRequest);
    m_requester->addCommand(MagneticDeclinationRequest);
    m_requester->startRequest();

    connect(m_requester, SIGNAL(translateData(QByteArray)), m_parser, SLOT(parseReply(QByteArray)));
    connect(m_parser, SIGNAL(dataReady(QByteArray, quint8)), this, SLOT(processData(QByteArray, quint8)));
    connect(m_parser, SIGNAL(lastAnswer(QByteArray)), this, SLOT(onLastAnswer(QByteArray)));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
}

double CompassLCC5000Device::getHeading() {
    return m_heading;
}

double CompassLCC5000Device::getPitch() {
    return m_pitch;
}

double CompassLCC5000Device::getRoll() {
    return m_roll;
}

double CompassLCC5000Device::getMagneticDeclination() {
    return m_magneticDeclination;
}

quint32 CompassLCC5000Device::getBaudRate() {
    return m_baudRate;
}

void CompassLCC5000Device::setBaudRate(quint32 baud) {
    BaudRateCommand->setV<quint32>(baud);
    m_requester->addExtraCommand(BaudRateCommand);
    m_requester->addExtraCommand(SaveSettingsCommand);
}

void CompassLCC5000Device::setMagneticDeclination(double value) {
    MagneticDeclinationCommand->setV<double>(value);
    m_requester->addExtraCommand(MagneticDeclinationCommand);
    m_requester->addExtraCommand(SaveSettingsCommand);
}

void CompassLCC5000Device::loadConfig() {
    QSettings settings(m_configPath, QSettings::IniFormat);
    settings.beginGroup(m_section);
    m_deviceAddr = settings.value("DeviceAddress", 0x00).toUInt();
    settings.endGroup();
}

void CompassLCC5000Device::onTimer() {
    m_statusOnline = false;
}

void CompassLCC5000Device::processData(const QByteArray &data, quint8 cmdId) {

    m_statusOnline = true;

    if(cmdId == CompassResponces::ALLANGLE) {
        m_pitch = BcdConverter::toDouble_SXXXYY(data.mid(0, 3));
        m_roll = BcdConverter::toDouble_SXXXYY(data.mid(3, 3));
        m_heading = BcdConverter::toDouble_SXXXYY(data.mid(6, 3));
    } else if(cmdId == CompassResponces::ROLL) {
        m_roll = BcdConverter::toDouble_SXXXYY(data.mid(0, 3));
    } else if(cmdId == CompassResponces::HEADING) {
        m_heading = BcdConverter::toDouble_SXXXYY(data.mid(0, 3));
    } else if(cmdId == CompassResponces::PITCH) {
        m_pitch = BcdConverter::toDouble_SXXXYY(data.mid(0, 3));
    } else if(cmdId == CompassResponces::SETMAGNETICDECLINATION) {
        if(static_cast<quint8>(data[0]) == 0x00) {
            m_magneticDeclination = MagneticDeclinationCommand->getV<double>();
        } else if(static_cast<quint8>(data[0]) == 0xFF) {
            qDebug() << "Error while writing Magnetic declination: " << MagneticDeclinationCommand->getV<double>();
        }
    } else if(cmdId == CompassResponces::MAGNETICDECLINATION) {
        m_magneticDeclination = BcdConverter::toDouble_SXXY(data.mid(0, 2));
    } else if(cmdId == CompassResponces::BAUDRATE) {
        //m_baudRate = CompassBaud::CompassBaudMap.value(data[0]);
    } else if(cmdId == CompassResponces::SETMODULEADDRESS) {
        if(static_cast<quint8>(data[0]) == 0x00) {
            m_deviceAddr = ModuleAddressCommand->getV<quint8>();
        } else if(static_cast<quint8>(data[0]) == 0xFF) {
            qDebug() << "Error while writing module address: " << ModuleAddressCommand->getV<quint8>();
        }
    } else if(cmdId == CompassResponces::MODULEADDRESS) {
        m_deviceAddr = static_cast<quint8>(data[0]);
    } else if(cmdId == CompassResponces::SETOUTPUTANGLEMODE) {

    } else if(cmdId == CompassResponces::SAVESETTINGS) {
        if(static_cast<quint8>(data[0]) == 0x00) {
            qDebug() << "Successful saving settings!";
        } else if(static_cast<quint8>(data[0]) == 0xFF) {
            qDebug() << "Error while saving settings!";
        }
    } else if(cmdId == CompassResponces::SWITCHCALIBRATIONOUTPUT) {
        m_calibrationOutput = static_cast<quint8>(data[0]);
    }

    m_timer->start(1000);
}

void CompassLCC5000Device::onLastAnswer(const QByteArray &packet) {
    m_lastAnswer = packet;
}
