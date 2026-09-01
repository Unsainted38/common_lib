#ifndef MSHPRDEVICE_H
#define MSHPRDEVICE_H

#include <QObject>
#include "requesters/serial_circular_requester.h"
#include "cmd/abstract_command.h"
#include "parsers/mshpr_parser.h"

/**
 * @brief Содержит строковые идентификаторы команд МШПР.
 */
struct MShPR_COMMANDS {
    inline static const QString STATUS_CMD = "STA"; /**< Хранит status cmd. */
    inline static const QString GETERODIN_CMD = "FKA"; /**< Хранит geterodin cmd. */
    inline static const QString ATTENUATION_CMD = "AT1"; /**< Хранит attenuation cmd. */
    inline static const QString ADDRESS_CMD = "ADR"; /**< Хранит address cmd. */
    inline static const QString BAUD_CMD = "BRG"; /**< Хранит baud cmd. */
};


/**
 * @brief Перечисляет коды скоростей обмена МШПР.
 */
enum MShPRBaud {
    Baud4800 = 0,
    Baud9600 = 1,
    Baud19200 = 2,
    Baud38400 = 3
};


/**
 * @brief Предоставляет высокоуровневый интерфейс управления устройством МШПР.
 */
class MShPRDevice : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Предоставляет высокоуровневый интерфейс управления устройством МШПР.
     *
     * @param requester Requester, выполняющий команды устройства.
     * @param configPath Путь к INI-файлу конфигурации.
     * @param section Имя секции с параметрами объекта.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit MShPRDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent = nullptr);
    /**
     * @brief Загружает параметры из указанной секции INI-файла.
     */
void loadConfig();
    /**
     * @brief Возвращает attenuation.
     *
     * @return Текущее значение параметра.
     */
quint8 getAttenuation();
    /**
     * @brief Возвращает geterodin.
     *
     * @return Текущее значение параметра.
     */
quint8 getGeterodin();
    /**
     * @brief Возвращает status.
     *
     * @return Текущее значение параметра.
     */
quint8 getStatus();
    /**
     * @brief Возвращает device address.
     *
     * @return Текущее значение параметра.
     */
quint8 getDeviceAddress();
    /**
     * @brief Возвращает last answer.
     *
     * @return Текущее значение параметра.
     */
QString getLastAnswer();
    /**
     * @brief Устанавливает attenuation.
     *
     * @param value Новое значение параметра.
     */
void setAttenuation(quint8 value);
    /**
     * @brief Устанавливает geterodin.
     *
     * @param value Новое значение параметра.
     */
void setGeterodin(quint8 value);
    /**
     * @brief Устанавливает address.
     *
     * @param value Новое значение параметра.
     */
void setAddress(quint8 value);
    /**
     * @brief Устанавливает baud.
     *
     * @param baud Код или числовое значение скорости обмена.
     */
void setBaud(MShPRBaud baud);
    /**
     * @brief Устанавливает baud.
     *
     * @param baud Код или числовое значение скорости обмена.
     */
void setBaud(quint8 baud);

signals:
private:


    QString m_configPath; /**< Путь к INI-файлу конфигурации. */
    QString m_section; /**< Секция INI-файла для этого объекта. */
    SerialCircularRequester *m_requester; /**< Requester, выполняющий команды устройства. */
    MShPRParser *m_parser; /**< Потоковый парсер ответов устройства. */
    AbstractCommand *StatusCommand, *AttenuationCommand, *GeterodinCommand, *AddressCommand, *BaudCommand; /**< Команда или набор команд baud command. */
    QString m_deviceAddr = "01"; /**< Адрес device addr. */
    quint8 m_status = 0; /**< Хранит status. */
    quint8 m_attenuation = 0; /**< Хранит attenuation. */
    quint8 m_geterodin = 0; /**< Хранит geterodin. */
    quint8 m_baud = MShPRBaud::Baud19200; /**< Хранит baud. */
    QString m_lastAnswer; /**< Последний выделенный ответ устройства. */
    QTimer *m_timer; /**< Таймер контроля активности устройства. */
private slots:
    /**
     * @brief Декодирует полезную нагрузку ответа и обновляет значение команды.
     *
     * @param addr Адрес устройства в формате протокола.
     * @param fieldsMap Разобранные поля ответа по их строковым идентификаторам.
     */
void processData(QString addr, QMap<QString, int> fieldsMap);
    /**
     * @brief Обрабатывает событие timer.
     */
void onTimer();
    void LastAnswer(QByteArray packet);
};

#endif // MSHPRDEVICE_H
