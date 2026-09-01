#ifndef MODBUS_PROTOCOL_FACTORY_H
#define MODBUS_PROTOCOL_FACTORY_H

#include <QObject>
#include <QSettings>
#include <modbus/abstract_modbus_protocol.h>
#include <modbus/modbus_rtu.h>
#include <modbus/modbus_tcp.h>

/**
 * @brief Создаёт реализацию Modbus RTU, TCP или RTU-over-TCP из настроек.
 */
class ModBusProtocolFactory : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Создаёт реализацию Modbus RTU, TCP или RTU-over-TCP из настроек.
     *
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit ModBusProtocolFactory(QObject *parent = nullptr);
    /**
     * @brief Создаёт объект требуемой реализации по типу или настройкам.
     *
     * @param protocolType Строковый тип протокола: rtu, tcp или viatcp.
     * @param slaveID Адрес ведомого Modbus-устройства.
     * @return Указатель на созданный объект; для неизвестного транспорта может быть nullptr.
     */
static AbstractModBusProtocol* getInstance(QString protocolType, quint8 slaveID);
    /**
     * @brief Создаёт объект требуемой реализации по типу или настройкам.
     *
     * @param configPath Путь к INI-файлу конфигурации.
     * @param section Имя секции с параметрами объекта.
     * @return Указатель на созданный объект; для неизвестного транспорта может быть nullptr.
     */
static AbstractModBusProtocol* getInstance(QString configPath, QString section);
signals:
};

#endif // MODBUS_PROTOCOL_FACTORY_H
