#ifndef MODBUS_DEVICE_H
#define MODBUS_DEVICE_H

#include <QObject>
#include <requesters/serial_circular_requester.h>
#include <utilities/config_helper.h>
#include <modbus/modbus_protocol_factory.h>
#include <memory>

/**
 * @brief Представляет Modbus-устройство с фабричным созданием протокола.
 */
class ModbusDevice : public QObject
{
    Q_OBJECT
    quint8 device_id_; /**< Хранит device id. */
    std::shared_ptr<SerialCircularRequester> requester_; /**< Requester, выполняющий команды устройства. */
    QByteArray buffer_; /**< Накопительный буфер входных данных. */

protected:
    AbstractModBusProtocol *protocol_; /**< Реализация упаковки и разбора Modbus. */
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
/**
     * @brief Добавляет команду для однократного выполнения без ожидания ответа.
     *
     * @param cmd Команда для однократного выполнения.
     */
void executeNoResponceCommand(AbstractCommand *cmd);

public:
    /**
     * @brief Представляет Modbus-устройство с фабричным созданием протокола.
     *
     * @param requester Requester, выполняющий команды устройства.
     * @param configPath Путь к INI-файлу конфигурации.
     * @param section Имя секции с параметрами объекта.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit ModbusDevice(std::shared_ptr<SerialCircularRequester> requester, QString configPath, QString section, QObject *parent = nullptr);
    /**
     * @brief Возвращает адрес устройства на шине Modbus.
     *
     * @return Адрес устройства.
     */
quint8 deviceAddress() const;

signals:

private slots:
};

#endif // MODBUS_DEVICE_H
