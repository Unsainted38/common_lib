#ifndef COMPASSLCC5000DEVICE_H
#define COMPASSLCC5000DEVICE_H

#include <QObject>
#include "requesters/serial_circular_requester.h"
#include "cmd/abstract_command.h"
#include "parsers/compass_lcc5000_parser.h"
#include <QTimer>

/**
 * @brief Содержит коды запросов компаса LC-C5000.
 */
struct CompassCommands {
    inline static const quint8 PITCH = 0x01; /**< Хранит pitch. */
    inline static const quint8 ROLL = 0x02; /**< Хранит roll. */
    inline static const quint8 HEADING = 0x03; /**< Хранит heading. */
    inline static const quint8 ALLANGLE = 0x04; /**< Хранит allangle. */
    inline static const quint8 SETMAGNETICDECLINATION = 0x06; /**< Хранит setmagneticdeclination. */
    inline static const quint8 MAGNETICDECLINATION = 0x07; /**< Хранит magneticdeclination. */
    inline static const quint8 BAUDRATE = 0x0B; /**< Хранит baudrate. */
    inline static const quint8 SETMODULEADDRESS = 0x0F; /**< Хранит setmoduleaddress. */
    inline static const quint8 MODULEADDRESS = 0x1F; /**< Хранит moduleaddress. */
    inline static const quint8 SETOUTPUTANGLEMODE = 0x0C; /**< Хранит setoutputanglemode. */
    inline static const quint8 SAVESETTINGS = 0x0A; /**< Хранит savesettings. */
    inline static const quint8 SWITCHCALIBRATIONOUTPUT = 0xA3; /**< Хранит switchcalibrationoutput. */
};
/**
 * @brief Содержит коды ответов компаса LC-C5000.
 */
struct CompassResponces {
    inline static const quint8 PITCH = 0x81; /**< Хранит pitch. */
    inline static const quint8 ROLL = 0x82; /**< Хранит roll. */
    inline static const quint8 HEADING = 0x83; /**< Хранит heading. */
    inline static const quint8 ALLANGLE = 0x84; /**< Хранит allangle. */
    inline static const quint8 SETMAGNETICDECLINATION = 0x86; /**< Хранит setmagneticdeclination. */
    inline static const quint8 MAGNETICDECLINATION = 0x87; /**< Хранит magneticdeclination. */
    inline static const quint8 BAUDRATE = 0x8B; /**< Хранит baudrate. */
    inline static const quint8 SETMODULEADDRESS = 0x8F; /**< Хранит setmoduleaddress. */
    inline static const quint8 MODULEADDRESS = 0x1F; /**< Хранит moduleaddress. */
    inline static const quint8 SETOUTPUTANGLEMODE = 0x8C; /**< Хранит setoutputanglemode. */
    inline static const quint8 SAVESETTINGS = 0x8A; /**< Хранит savesettings. */
    inline static const quint8 SWITCHCALIBRATIONOUTPUT = 0xA3; /**< Хранит switchcalibrationoutput. */
};

/**
 * @brief Содержит поддерживаемые скорости обмена компаса.
 */
struct CompassBaud {
    inline static const quint32 BAUD2400 = 2400; /**< Хранит baud2400. */
    inline static const quint32 BAUD4800 = 4800; /**< Хранит baud4800. */
    inline static const quint32 BAUD9600 = 9600; /**< Хранит baud9600. */
    inline static const quint32 BAUD19200 = 19200; /**< Хранит baud19200. */
    inline static const quint32 BAUD115200 = 115200; /**< Хранит baud115200. */
    inline static const quint32 BAUD38400 = 38400; /**< Хранит baud38400. */
    inline static const quint32 BAUD57600 = 57600; /**< Хранит baud57600. */
    //    inline static const QMap<quint8, quint32> *CompassBaudMap = new QMap<quint8, quint32>({
    //        {0x00, BAUD2400}, {0x01, BAUD4800},
    //        {0x02, BAUD9600}, {0x03, BAUD19200},
    //        {0x04, BAUD115200}, {0x05, BAUD38400},
    //        {0x06, BAUD57600}
    //    });
};

/**
 * @brief Предоставляет опрос и настройку компаса LC-C5000.
 */
class CompassLCC5000Device : public QObject {
    Q_OBJECT
    QString m_name = "Compass LC-C5000"; /**< Хранит name. */
    SerialCircularRequester *m_requester; /**< Requester, выполняющий команды устройства. */
    AbstractCommand *AllAnglesRequest, *PitchRequest, *RollRequest, *HeadingRequest, /**< Команды чтения углов ориентации. */
                    *MagneticDeclinationRequest, *MagneticDeclinationCommand, *BaudRateCommand, /**< Команды магнитного склонения и скорости обмена. */
                    *ModuleAddressCommand, *CurrentAddressRequest, *OutputAngleModeCommand, *SaveSettingsCommand, /**< Команды адреса, режима вывода и сохранения настроек. */
                    *SwitchCalibrationOutpuRequest; /**< Команда или набор команд switch calibration outpu request. */
    CompassLCC5000Parser *m_parser; /**< Потоковый парсер ответов устройства. */
    QString m_section = ""; /**< Секция INI-файла для этого объекта. */
    QString m_configPath = ""; /**< Путь к INI-файлу конфигурации. */
    quint8 m_deviceAddr = 0x00; /**< Адрес device addr. */
    QTimer *m_timer; /**< Таймер контроля активности устройства. */
    bool m_statusOnline = false; /**< Признак недавнего корректного ответа устройства. */
    double m_heading = 0.0; /**< Хранит heading. */
    double m_pitch = 0.0; /**< Хранит pitch. */
    double m_roll = 0.0; /**< Хранит roll. */
    double m_magneticDeclination = 0.0; /**< Хранит magnetic declination. */
    quint32 m_baudRate = CompassBaud::BAUD9600; /**< Хранит baud rate. */
    quint8 m_calibrationOutput = 0x00; /**< Хранит calibration output. */
    QByteArray m_lastAnswer = ""; /**< Последний выделенный ответ устройства. */


public:
    /**
     * @brief Предоставляет опрос и настройку компаса LC-C5000.
     *
     * @param requester Requester, выполняющий команды устройства.
     * @param configPath Путь к INI-файлу конфигурации.
     * @param section Имя секции с параметрами объекта.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit CompassLCC5000Device(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent = nullptr);
    /**
     * @brief Возвращает heading.
     *
     * @return Текущее значение параметра.
     */
double getHeading();
    /**
     * @brief Возвращает pitch.
     *
     * @return Текущее значение параметра.
     */
double getPitch();
    /**
     * @brief Возвращает roll.
     *
     * @return Текущее значение параметра.
     */
double getRoll();
    /**
     * @brief Возвращает magnetic declination.
     *
     * @return Текущее значение параметра.
     */
double getMagneticDeclination();
    /**
     * @brief Возвращает baud rate.
     *
     * @return Текущее значение параметра.
     */
quint32 getBaudRate();
    /**
     * @brief Устанавливает baud rate.
     *
     * @param baud Код или числовое значение скорости обмена.
     */
void setBaudRate(quint32 baud);
    /**
     * @brief Устанавливает magnetic declination.
     *
     * @param value Новое значение параметра.
     */
void setMagneticDeclination(double value);


signals:
private:
    /**
     * @brief Загружает параметры из указанной секции INI-файла.
     */
void loadConfig();
private slots:
    /**
     * @brief Обрабатывает событие timer.
     */
void onTimer();
    /**
     * @brief Декодирует полезную нагрузку ответа и обновляет значение команды.
     *
     * @param data Входные данные или полезная нагрузка ответа.
     * @param cmdId Код команды протокола.
     */
void processData(const QByteArray &data, quint8 cmdId);
    void onLastAnswer(const QByteArray &packet);

};

#endif // COMPASSLCC5000DEVICE_H
