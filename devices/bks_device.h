#ifndef BKSDEVICE_H
#define BKSDEVICE_H

#include <QObject>
#include <QTimer>

#include "requesters/serial_circular_requester.h"
#include "cmd/abstract_command.h"
#include "parsers/mshpr_parser.h"

/**
 * @brief Перечисляет коды скоростей обмена БКС.
 */
enum BksBaud {
    Baud4800 = 0,
    Baud9600 = 1,
    Baud19200 = 2,
    Baud38400 = 3
};


/**
 * @brief Содержит строковые идентификаторы команд БКС.
 */
struct BKS_COMMANDS {
    inline static const QString STATUS_CMD = "STA"; /**< Хранит status cmd. */
    inline static const QString FC_LOW_FREQ_CMD = "FC1"; /**< Хранит fc low freq cmd. */
    inline static const QString FC_HIGH_FREQ_CMD = "FC2"; /**< Хранит fc high freq cmd. */
    inline static const QString FX_LOW_FREQ_CMD = "FX1"; /**< Хранит fx low freq cmd. */
    inline static const QString FX_HIGH_FREQ_CMD = "FX2"; /**< Хранит fx high freq cmd. */
    inline static const QString ADDRESS_CMD = "ADR"; /**< Хранит address cmd. */
    inline static const QString BAUD_CMD = "BRG"; /**< Хранит baud cmd. */
};
/**
 * @brief Предоставляет высокоуровневый интерфейс управления устройством БКС.
 */
class BksDevice : public QObject {
    Q_OBJECT

    QString m_configPath; /**< Путь к INI-файлу конфигурации. */
    QString m_section; /**< Секция INI-файла для этого объекта. */
    SerialCircularRequester *m_requester; /**< Requester, выполняющий команды устройства. */
    MShPRParser *m_parser; /**< Потоковый парсер ответов устройства. */
    AbstractCommand *StatusCommand, *FC1Command, *FC2Command, *FX1Command, *FX2Command, *AddressCommand, *BaudCommand; /**< Команда или набор команд baud command. */
    QString m_deviceAddr = "00"; /**< Адрес device addr. */
    bool m_statusOnline = false; /**< Признак недавнего корректного ответа устройства. */
    quint8 m_FC1 = 0; /**< Хранит fc1. */
    quint8 m_FC2 = 0; /**< Хранит fc2. */
    quint8 m_FX1 = 0; /**< Хранит fx1. */
    quint8 m_FX2 = 0; /**< Хранит fx2. */
    quint8 m_baud = BksBaud::Baud19200; /**< Хранит baud. */
    QString m_lastAnswer; /**< Последний выделенный ответ устройства. */
    QTimer *m_timer; /**< Таймер контроля активности устройства. */
public:
    /**
     * @brief Предоставляет высокоуровневый интерфейс управления устройством БКС.
     *
     * @param requester Requester, выполняющий команды устройства.
     * @param configPath Путь к INI-файлу конфигурации.
     * @param section Имя секции с параметрами объекта.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit BksDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent = nullptr);
    /**
     * @brief Загружает параметры из указанной секции INI-файла.
     */
void loadConfig();
    /**
     * @brief Возвращает fc1.
     *
     * @return Текущее значение параметра.
     */
quint8 getFC1();
    /**
     * @brief Возвращает fc2.
     *
     * @return Текущее значение параметра.
     */
quint8 getFC2();
    /**
     * @brief Возвращает fx1.
     *
     * @return Текущее значение параметра.
     */
quint8 getFX1();
    /**
     * @brief Возвращает fx2.
     *
     * @return Текущее значение параметра.
     */
quint8 getFX2();
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
     * @brief Возвращает status online.
     *
     * @return Текущее логическое состояние.
     */
bool getStatusOnline();
    /**
     * @brief Устанавливает fc1.
     *
     * @param FC1 Нижняя частота первого канала.
     */
void setFC1(quint8 FC1);
    /**
     * @brief Устанавливает fc2.
     *
     * @param FC2 Верхняя частота первого канала.
     */
void setFC2(quint8 FC2);
    /**
     * @brief Устанавливает fx1.
     *
     * @param FX1 Нижняя частота второго канала.
     */
void setFX1(quint8 FX1);
    /**
     * @brief Устанавливает fx2.
     *
     * @param FX2 Верхняя частота второго канала.
     */
void setFX2(quint8 FX2);
    /**
     * @brief Устанавливает device address.
     *
     * @param value Новое значение параметра.
     */
void setDeviceAddress(quint8 value);
    /**
     * @brief Устанавливает baud.
     *
     * @param baud Код или числовое значение скорости обмена.
     */
void setBaud(BksBaud baud);
    /**
     * @brief Устанавливает baud.
     *
     * @param baud Код или числовое значение скорости обмена.
     */
void setBaud(quint8 baud);
signals:

private:

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

#endif // BKSDEVICE_H
