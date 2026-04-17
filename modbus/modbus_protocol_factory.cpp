#include "modbus_protocol_factory.h"

ModBusProtocolFactory::ModBusProtocolFactory(QObject *parent)
    : QObject(parent)
{}

AbstractModBusProtocol *ModBusProtocolFactory::getInstance(QString protocolType, quint8 slaveID)
{
    if (protocolType == "rtu") {
        return new ModBusRtu(slaveID);
    } else if (protocolType == "tcp") {
        return new ModBusTcp(slaveID);
    } else if (protocolType == "viatcp") {

    }
}

AbstractModBusProtocol *ModBusProtocolFactory::getInstance(QString configPath, QString section)
{
    QSettings settings(configPath, QSettings::IniFormat);
    settings.beginGroup(section);
    QString protocolType = settings.value("ProtocolType", "rtu").toString();
    quint8 slaveID = settings.value("slaveID", 0).toUInt();
    return getInstance(protocolType, slaveID);
}
