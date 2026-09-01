#ifndef UBPCHDEVICE_H
#define UBPCHDEVICE_H

#include <QObject>
#include "requesters/serial_circular_requester.h"
#include "cmd/abstract_command.h"
#include "parsers/ubpch_parser.h"
#include <QTimer>

/**
 * @brief Предоставляет опрос и настройку устройства УБПЧ.
 */
class UBPChDevice : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Предоставляет опрос и настройку устройства УБПЧ.
     *
     * @param requester Requester, выполняющий команды устройства.
     * @param configPath Путь к INI-файлу конфигурации.
     * @param section Имя секции с параметрами объекта.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit UBPChDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent = nullptr);
    /**
     * @brief Загружает параметры из указанной секции INI-файла.
     */
void loadConfig();
    /**
     * @brief Устанавливает enabled.
     *
     * @param value Новое значение параметра.
     */
void setEnabled(quint8 value);
    /**
     * @brief Возвращает enabled.
     *
     * @return Текущее значение параметра.
     */
quint8 getEnabled();
    /**
     * @brief Устанавливает attenuation.
     *
     * @param value Новое значение параметра.
     */
void setAttenuation(quint8 value);
    /**
     * @brief Возвращает attenuation.
     *
     * @return Текущее значение параметра.
     */
quint32 getAttenuation();
    /**
     * @brief Возвращает error status.
     *
     * @return Текущее значение параметра.
     */
quint8 getErrorStatus();
    /**
     * @brief Возвращает device status.
     *
     * @return Текущее значение параметра.
     */
quint8 getDeviceStatus();
    /**
     * @brief Возвращает output power.
     *
     * @return Текущее значение параметра.
     */
float getOutputPower();
    /**
     * @brief Возвращает empower.
     *
     * @return Текущее значение параметра.
     */
float getEmpower();
    /**
     * @brief Возвращает temperature.
     *
     * @return Текущее значение параметра.
     */
qint32 getTemperature();
    /**
     * @brief Возвращает voltage.
     *
     * @return Текущее значение параметра.
     */
float getVoltage();
    /**
     * @brief Возвращает status online.
     *
     * @return Текущее логическое состояние.
     */
bool getStatusOnline();

    /**
     * @brief Возвращает last answer.
     *
     * @return Текущее значение параметра.
     */
QByteArray getLastAnswer();
signals:
private:
    QString m_configPath; /**< Путь к INI-файлу конфигурации. */
    QString m_section; /**< Секция INI-файла для этого объекта. */
    SerialCircularRequester *m_requester; /**< Requester, выполняющий команды устройства. */
    UBPChParser *m_parser; /**< Потоковый парсер ответов устройства. */
    AbstractCommand *EnableCommand, *AttenuationCommand, *ErrorStatusCommand, *DeviceStatusCommand, /**< Команды циклического чтения основных параметров. */
                    *OutputPowerCommand, *EmpowerCommand, *TemperatureCommand, *VoltageCommand; /**< Команда или набор команд voltage command. */
    AbstractCommand *EnableWriteCommand, *AttenuationWriteCommand; /**< Команда или набор команд attenuation write command. */
    quint32 m_attenuation; /**< Хранит attenuation. */
    quint8 m_enabled; /**< Хранит enabled. */
    quint8 m_deviceStatus; /**< Состояние device status. */
    quint8 m_errorStatus; /**< Состояние error status. */
    quint32 m_outputPower; /**< Хранит output power. */
    quint32 m_empower; /**< Хранит empower. */
    qint32 m_temperature; /**< Хранит temperature. */
    float m_voltage; /**< Хранит voltage. */
    quint8 m_deviceAddr = 0x16; /**< Адрес device addr. */
    quint8 m_masterAddr = 0x01; /**< Адрес master addr. */
    QEventLoop loop; /**< Хранит loop. */
    QTimer *m_timer; /**< Таймер контроля активности устройства. */
    bool m_statusOnline = false; /**< Признак недавнего корректного ответа устройства. */
    QByteArray m_lastAnswer; /**< Последний выделенный ответ устройства. */
private slots:
    /**
     * @brief Обрабатывает событие timer.
     */
void onTimer();
    /**
     * @brief Обрабатывает событие status online.
     */
void onStatusOnline();
public slots:
    void onLastAnswer(QByteArray packet);
};

#endif // UBPCHDEVICE_H
