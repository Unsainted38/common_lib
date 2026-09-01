#ifndef ABSTRACT_MODBUS_DEVICE_H
#define ABSTRACT_MODBUS_DEVICE_H

#include <QObject>
#include <requesters/serial_circular_requester.h>
#include <utilities/config_helper.h>
#include <modbus/read_holding_registers.h>
#include <modbus/modbus_protocol_factory.h>

/**
 * @brief Базовый класс Modbus-устройства с фабричным созданием протокола.
 */
class AbstractModbusDevice : public QObject
{
    Q_OBJECT
    quint8 deviceID; /**< Хранит device id. */
    QString configPath; /**< Хранит config path. */
    QString section; /**< Хранит section. */
    SerialCircularRequester *requester; /**< Requester, выполняющий команды устройства. */
    QByteArray buffer; /**< Накопительный буфер входных данных. */

protected:
    AbstractModBusProtocol *protocol; /**< Реализация упаковки и разбора Modbus. */
    /**
     * @brief Добавляет переиспользуемую команду в циклический опрос.
     *
     * @param cmd Команда, добавляемая в циклический опрос.
     */
void addCircularCommand(AbstractCommand *cmd);
    /**
     * @brief Добавляет команду для однократного выполнения.
     *
     * @param cmd Команда для однократного выполнения.
     */
void executeCommand(AbstractCommand *cmd);

public:
    /**
     * @brief Базовый класс Modbus-устройства с фабричным созданием протокола.
     *
     * @param requester Requester, выполняющий команды устройства.
     * @param configPath Путь к INI-файлу конфигурации.
     * @param section Имя секции с параметрами объекта.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit AbstractModbusDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent = nullptr);
    /**
     * @brief Возвращает адрес устройства на шине Modbus.
     *
     * @return Адрес устройства.
     */
quint8 deviceAddress();

signals:

private slots:
};

#endif // ABSTRACT_MODBUS_DEVICE_H
