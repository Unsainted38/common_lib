#ifndef ABSTRACTNETWORKTRANSPORTFACTORY_H
#define ABSTRACTNETWORKTRANSPORTFACTORY_H

#include <QObject>
#include "abstract_network_transport.h"

/**
 * @brief Создаёт сетевой или последовательный транспорт из INI-настроек.
 */
class AbstractNetworkTransportFactory : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Создаёт сетевой или последовательный транспорт из INI-настроек.
     *
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit AbstractNetworkTransportFactory(QObject *parent = nullptr);
    /**
     * @brief Создаёт объект требуемой реализации по типу или настройкам.
     *
     * @param InterfaceType Строковый тип транспорта: serial, tcp или udp.
     * @param fileName Путь к INI-файлу конфигурации.
     * @param section Имя секции с параметрами объекта.
     * @return Указатель на созданный объект; для неизвестного транспорта может быть nullptr.
     */
static AbstractNetworkTransport *getInstance(QString InterfaceType, QString fileName, QString section);
    /**
     * @brief Создаёт объект требуемой реализации по типу или настройкам.
     *
     * @param fileName Путь к INI-файлу конфигурации.
     * @param section Имя секции с параметрами объекта.
     * @return Указатель на созданный объект; для неизвестного транспорта может быть nullptr.
     */
static AbstractNetworkTransport *getInstance(QString fileName, QString section);

signals:

};

#endif // ABSTRACTNETWORKTRANSPORTFACTORY_H
