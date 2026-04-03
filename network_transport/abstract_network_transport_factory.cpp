#include "abstract_network_transport_factory.h"

AbstractNetworkTransportFactory::AbstractNetworkTransportFactory(QObject *parent)
    : QObject(parent) {
}

AbstractNetworkTransport *AbstractNetworkTransportFactory::getInstance(QString InterfaceType, QString fileName, QString section) {
    if(InterfaceType == "serial") {
        return new SerialTransport(fileName, section);
    } else if(InterfaceType == "tcp") {
        return new TcpTransport(fileName, section);
    } else if(InterfaceType == "udp") {

    }
}

AbstractNetworkTransport *AbstractNetworkTransportFactory::getInstance(QString fileName, QString section) {
    QSettings settings(fileName, QSettings::IniFormat);
    settings.beginGroup(section);
    QString InterfaceType = settings.value("InterfaceType", "serial").toString();
    settings.endGroup();
    return getInstance(InterfaceType, fileName, section);
}

