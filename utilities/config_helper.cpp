#include "config_helper.h"

ConfigHelper::ConfigHelper(QObject *parent)
    : QObject(parent) {

}

void ConfigHelper::loadTransportConfig(QString path, QString section)
{

}

quint8 ConfigHelper::loadModBusDeviceAddress(QString path, QString section)
{
    quint8 res;
    qDebug() << "Loading ModBus device address from" << path << "section:" << section;
    QSettings settings(path, QSettings::IniFormat);
    settings.beginGroup(section);
    res = settings.value("DeviceAddress", 0).toUInt();
    settings.endGroup();
    qDebug() << "Device address is:" << res;
    return res;
}
