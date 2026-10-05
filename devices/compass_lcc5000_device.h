#ifndef COMPASSLCC5000DEVICE_H
#define COMPASSLCC5000DEVICE_H

#include <QObject>
#include "serial_circular_requester.h"
#include "abstract_command.h"
#include "compass_lcc5000_parser.h"
#include <QTimer>

struct CompassCommands {
    static const quint8 PITCH;
    static const quint8 ROLL;
    static const quint8 HEADING;
    static const quint8 ALLANGLE;
    static const quint8 SETMAGNETICDECLINATION;
    static const quint8 MAGNETICDECLINATION;
    static const quint8 BAUDRATE;
    static const quint8 SETMODULEADDRESS;
    static const quint8 MODULEADDRESS;
    static const quint8 SETOUTPUTANGLEMODE;
    static const quint8 SAVESETTINGS;
    static const quint8 SWITCHCALIBRATIONOUTPUT;
};
struct CompassResponces {
    static const quint8 PITCH;
    static const quint8 ROLL;
    static const quint8 HEADING;
    static const quint8 ALLANGLE;
    static const quint8 SETMAGNETICDECLINATION;
    static const quint8 MAGNETICDECLINATION;
    static const quint8 BAUDRATE;
    static const quint8 SETMODULEADDRESS;
    static const quint8 MODULEADDRESS;
    static const quint8 SETOUTPUTANGLEMODE;
    static const quint8 SAVESETTINGS;
    static const quint8 SWITCHCALIBRATIONOUTPUT;
};

struct CompassBaud {
    static const quint32 BAUD2400;
    static const quint32 BAUD4800;
    static const quint32 BAUD9600;
    static const quint32 BAUD19200;
    static const quint32 BAUD115200;
    static const quint32 BAUD38400;
    static const quint32 BAUD57600;
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
    std::shared_ptr<SerialCircularRequester> m_requester;
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
    explicit CompassLCC5000Device(std::shared_ptr<SerialCircularRequester> requester, QString configPath, QString section, QObject *parent = nullptr);
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
