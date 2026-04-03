#ifndef ABSTRACTNETWORKTRANSPORTFACTORY_H
#define ABSTRACTNETWORKTRANSPORTFACTORY_H

#include <QObject>
#include "abstract_network_transport.h"
#include "serial_transport.h"
#include "tcp_transport.h"

class AbstractNetworkTransportFactory : public QObject {
    Q_OBJECT
public:
    explicit AbstractNetworkTransportFactory(QObject *parent = nullptr);
    static AbstractNetworkTransport *getInstance(QString InterfaceType, QString fileName, QString section);
    static AbstractNetworkTransport *getInstance(QString fileName, QString section);

signals:

};

#endif // ABSTRACTNETWORKTRANSPORTFACTORY_H
