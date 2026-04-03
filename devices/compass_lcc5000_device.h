#ifndef COMPASSLCC5000DEVICE_H
#define COMPASSLCC5000DEVICE_H

#include <QObject>
#include "requesters/serial_circular_requester.h"
#include "cmd/abstract_command.h"
#include "parsers/compass_lcc5000_parser.h"
#include "cmd/compass_lcc5000_command.h"
#include <QTimer>

struct CompassCommands {
    inline static const quint8 PITCH = 0x01;
    inline static const quint8 ROLL = 0x02;
    inline static const quint8 HEADING = 0x03;
    inline static const quint8 ALLANGLE = 0x04;
    inline static const quint8 SETMAGNETICDECLINATION = 0x06;
    inline static const quint8 MAGNETICDECLINATION = 0x07;
    inline static const quint8 BAUDRATE = 0x0B;
    inline static const quint8 SETMODULEADDRESS = 0x0F;
    inline static const quint8 MODULEADDRESS = 0x1F;
    inline static const quint8 SETOUTPUTANGLEMODE = 0x0C;
    inline static const quint8 SAVESETTINGS = 0x0A;
    inline static const quint8 SWITCHCALIBRATIONOUTPUT = 0xA3;
};
struct CompassResponces {
    inline static const quint8 PITCH = 0x81;
    inline static const quint8 ROLL = 0x82;
    inline static const quint8 HEADING = 0x83;
    inline static const quint8 ALLANGLE = 0x84;
    inline static const quint8 SETMAGNETICDECLINATION = 0x86;
    inline static const quint8 MAGNETICDECLINATION = 0x87;
    inline static const quint8 BAUDRATE = 0x8B;
    inline static const quint8 SETMODULEADDRESS = 0x8F;
    inline static const quint8 MODULEADDRESS = 0x1F;
    inline static const quint8 SETOUTPUTANGLEMODE = 0x8C;
    inline static const quint8 SAVESETTINGS = 0x8A;
    inline static const quint8 SWITCHCALIBRATIONOUTPUT = 0xA3;
};

struct CompassBaud {
    inline static const quint32 BAUD2400 = 2400;
    inline static const quint32 BAUD4800 = 4800;
    inline static const quint32 BAUD9600 = 9600;
    inline static const quint32 BAUD19200 = 19200;
    inline static const quint32 BAUD115200 = 115200;
    inline static const quint32 BAUD38400 = 38400;
    inline static const quint32 BAUD57600 = 57600;
    //    inline static const QMap<quint8, quint32> *CompassBaudMap = new QMap<quint8, quint32>({
    //        {0x00, BAUD2400}, {0x01, BAUD4800},
    //        {0x02, BAUD9600}, {0x03, BAUD19200},
    //        {0x04, BAUD115200}, {0x05, BAUD38400},
    //        {0x06, BAUD57600}
    //    });
};

class CompassLCC5000Device : public QObject {
    Q_OBJECT
    QString m_name = "Compass LC-C5000";
    SerialCircularRequester *m_requester;
    AbstractCommand *AllAnglesRequest, *PitchRequest, *RollRequest, *HeadingRequest,
                    *MagneticDeclinationRequest, *MagneticDeclinationCommand, *BaudRateCommand,
                    *ModuleAddressCommand, *CurrentAddressRequest, *OutputAngleModeCommand, *SaveSettingsCommand,
                    *SwitchCalibrationOutpuRequest;
    CompassLCC5000Parser *m_parser;
    QString m_section = "";
    QString m_configPath = "";
    quint8 m_deviceAddr = 0x00;
    QTimer *m_timer;
    bool m_statusOnline = false;
    double m_heading = 0.0;
    double m_pitch = 0.0;
    double m_roll = 0.0;
    double m_magneticDeclination = 0.0;
    quint32 m_baudRate = CompassBaud::BAUD9600;
    quint8 m_calibrationOutput = 0x00;
    QByteArray m_lastAnswer = "";


public:
    explicit CompassLCC5000Device(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent = nullptr);
    double getHeading();
    double getPitch();
    double getRoll();
    double getMagneticDeclination();
    quint32 getBaudRate();
    void setBaudRate(quint32 baud);
    void setMagneticDeclination(double value);


signals:
private:
    void loadConfig();
private slots:
    void onTimer();
    void processData(const QByteArray &data, quint8 cmdId);
    void onLastAnswer(const QByteArray &packet);

};

#endif // COMPASSLCC5000DEVICE_H
