#ifndef MODBUS_PROTOCOL_FACTORY_H
#define MODBUS_PROTOCOL_FACTORY_H

#include <QObject>
#include <QSettings>
#include <modbus/abstract_modbus_protocol.h>
#include <modbus/modbus_rtu.h>
#include <modbus/modbus_tcp.h>

class ModBusProtocolFactory : public QObject
{
    Q_OBJECT
public:
    explicit ModBusProtocolFactory(QObject *parent = nullptr);
    static AbstractModBusProtocol* getInstance(QString protocolType, quint8 slaveID);
    static AbstractModBusProtocol* getInstance(QString configPath, QString section);
signals:
};

#endif // MODBUS_PROTOCOL_FACTORY_H
