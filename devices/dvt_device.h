#ifndef DVT_DEVICE_H
#define DVT_DEVICE_H

#include <QObject>
#include <QTimer>
#include <modbus/abstract_modbus_device.h>

/**
 * @brief Содержит последний измеренный набор параметров датчика ДВТ.
 */
struct DvtState {
    quint8 status; /**< Хранит status. */
    float pressure; /**< Хранит pressure. */
    float tempetature; /**< Хранит tempetature. */
    float humidity; /**< Хранит humidity. */
};

/**
 * @brief Предоставляет значения состояния датчика ДВТ через Modbus.
 */
class DvtDevice : public AbstractModbusDevice
{

    AbstractCommand *dvtStateCommand; /**< Команда или набор команд dvt state command. */
    quint8 deviceAddress; /**< Адрес device address. */
    DvtState dvtInfo{}; /**< Последнее разобранное состояние датчика. */
    QTimer *m_timer; /**< Таймер контроля активности устройства. */

public:
    /**
     * @brief Предоставляет значения состояния датчика ДВТ через Modbus.
     *
     * @param requester Requester, выполняющий команды устройства.
     * @param configPath Путь к INI-файлу конфигурации.
     * @param section Имя секции с параметрами объекта.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit DvtDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent = nullptr);
    /**
     * @brief Возвращает код состояния датчика.
     *
     * @return Текущее значение параметра.
     */
quint8 status();
    /**
     * @brief Возвращает измеренное давление.
     *
     * @return Результат операции типа float.
     */
float pressure();
    /**
     * @brief Возвращает измеренную температуру.
     *
     * @return Результат операции типа float.
     */
float temperature();
    /**
     * @brief Возвращает измеренную влажность.
     *
     * @return Результат операции типа float.
     */
float humidity();
signals:
private slots:
    /**
     * @brief Обрабатывает событие timer.
     */
void onTimer();
    /**
     * @brief Сохраняет последний ответ устройства для диагностики.
     *
     * @param packet Принятый пакет ответа устройства.
     */
void LastAnswer(QByteArray packet);
};

#endif // DVT_DEVICE_H
