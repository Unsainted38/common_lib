#ifndef COMPASSLCC5000DEVICE_H
#define COMPASSLCC5000DEVICE_H

#include <QObject>
#include "requesters/serial_circular_requester.h"
#include "cmd/abstract_command.h"
#include "parsers/compass_lcc5000_parser.h"
#include <QTimer>

/**
 * @brief
 *
 */
struct CompassCommands {
    inline static const quint8 PITCH = 0x01; /**< TODO: describe */
    inline static const quint8 ROLL = 0x02; /**< TODO: describe */
    inline static const quint8 HEADING = 0x03; /**< TODO: describe */
    inline static const quint8 ALLANGLE = 0x04; /**< TODO: describe */
    inline static const quint8 SETMAGNETICDECLINATION = 0x06; /**< TODO: describe */
    inline static const quint8 MAGNETICDECLINATION = 0x07; /**< TODO: describe */
    inline static const quint8 BAUDRATE = 0x0B; /**< TODO: describe */
    inline static const quint8 SETMODULEADDRESS = 0x0F; /**< TODO: describe */
    inline static const quint8 MODULEADDRESS = 0x1F; /**< TODO: describe */
    inline static const quint8 SETOUTPUTANGLEMODE = 0x0C; /**< TODO: describe */
    inline static const quint8 SAVESETTINGS = 0x0A; /**< TODO: describe */
    inline static const quint8 SWITCHCALIBRATIONOUTPUT = 0xA3; /**< TODO: describe */
};
/**
 * @brief
 *
 */
struct CompassResponces {
    inline static const quint8 PITCH = 0x81; /**< TODO: describe */
    inline static const quint8 ROLL = 0x82; /**< TODO: describe */
    inline static const quint8 HEADING = 0x83; /**< TODO: describe */
    inline static const quint8 ALLANGLE = 0x84; /**< TODO: describe */
    inline static const quint8 SETMAGNETICDECLINATION = 0x86; /**< TODO: describe */
    inline static const quint8 MAGNETICDECLINATION = 0x87; /**< TODO: describe */
    inline static const quint8 BAUDRATE = 0x8B; /**< TODO: describe */
    inline static const quint8 SETMODULEADDRESS = 0x8F; /**< TODO: describe */
    inline static const quint8 MODULEADDRESS = 0x1F; /**< TODO: describe */
    inline static const quint8 SETOUTPUTANGLEMODE = 0x8C; /**< TODO: describe */
    inline static const quint8 SAVESETTINGS = 0x8A; /**< TODO: describe */
    inline static const quint8 SWITCHCALIBRATIONOUTPUT = 0xA3; /**< TODO: describe */
};

/**
 * @brief
 *
 */
struct CompassBaud {
    inline static const quint32 BAUD2400 = 2400; /**< TODO: describe */
    inline static const quint32 BAUD4800 = 4800; /**< TODO: describe */
    inline static const quint32 BAUD9600 = 9600; /**< TODO: describe */
    inline static const quint32 BAUD19200 = 19200; /**< TODO: describe */
    inline static const quint32 BAUD115200 = 115200; /**< TODO: describe */
    inline static const quint32 BAUD38400 = 38400; /**< TODO: describe */
    inline static const quint32 BAUD57600 = 57600; /**< TODO: describe */
    //    inline static const QMap<quint8, quint32> *CompassBaudMap = new QMap<quint8, quint32>({
    //        {0x00, BAUD2400}, {0x01, BAUD4800},
    //        {0x02, BAUD9600}, {0x03, BAUD19200},
    //        {0x04, BAUD115200}, {0x05, BAUD38400},
    //        {0x06, BAUD57600}
    //    });
};

/**
 * @brief
 *
 */
class CompassLCC5000Device : public QObject {
    Q_OBJECT
    QString m_name = "Compass LC-C5000"; /**< TODO: describe */
    SerialCircularRequester *m_requester; /**< TODO: describe */
    AbstractCommand *AllAnglesRequest, *PitchRequest, *RollRequest, *HeadingRequest, /**< TODO: describe */
                    *MagneticDeclinationRequest, *MagneticDeclinationCommand, *BaudRateCommand, /**< TODO: describe */
                    *ModuleAddressCommand, *CurrentAddressRequest, *OutputAngleModeCommand, *SaveSettingsCommand, /**< TODO: describe */
                    *SwitchCalibrationOutpuRequest; /**< TODO: describe */
    CompassLCC5000Parser *m_parser; /**< TODO: describe */
    QString m_section = ""; /**< TODO: describe */
    QString m_configPath = ""; /**< TODO: describe */
    quint8 m_deviceAddr = 0x00; /**< TODO: describe */
    QTimer *m_timer; /**< TODO: describe */
    bool m_statusOnline = false; /**< TODO: describe */
    double m_heading = 0.0; /**< TODO: describe */
    double m_pitch = 0.0; /**< TODO: describe */
    double m_roll = 0.0; /**< TODO: describe */
    double m_magneticDeclination = 0.0; /**< TODO: describe */
    quint32 m_baudRate = CompassBaud::BAUD9600; /**< TODO: describe */
    quint8 m_calibrationOutput = 0x00; /**< TODO: describe */
    QByteArray m_lastAnswer = ""; /**< TODO: describe */


public:
    /**
     * @brief
     *
     * @param requester
     * @param configPath
     * @param section
     * @param parent
     */
explicit CompassLCC5000Device(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent = nullptr);
    /**
     * @brief
     *
     * @return double
     */
double getHeading();
    /**
     * @brief
     *
     * @return double
     */
double getPitch();
    /**
     * @brief
     *
     * @return double
     */
double getRoll();
    /**
     * @brief
     *
     * @return double
     */
double getMagneticDeclination();
    /**
     * @brief
     *
     * @return quint32
     */
quint32 getBaudRate();
    /**
     * @brief
     *
     * @param baud
     */
void setBaudRate(quint32 baud);
    /**
     * @brief
     *
     * @param value
     */
void setMagneticDeclination(double value);


signals:
private:
    /**
     * @brief
     *
     */
void loadConfig();
private slots:
    /**
     * @brief
     *
     */
void onTimer();
    /**
     * @brief
     *
     * @param data
     * @param cmdId
     */
void processData(const QByteArray &data, quint8 cmdId);
    void onLastAnswer(const QByteArray &packet);

};

#endif // COMPASSLCC5000DEVICE_H
