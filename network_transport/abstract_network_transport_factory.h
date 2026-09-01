#ifndef ABSTRACTNETWORKTRANSPORTFACTORY_H
#define ABSTRACTNETWORKTRANSPORTFACTORY_H

#include <QObject>
#include "abstract_network_transport.h"

/**
 * @brief
 *
 */
class AbstractNetworkTransportFactory : public QObject {
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
explicit AbstractNetworkTransportFactory(QObject *parent = nullptr);
    /**
     * @brief
     *
     * @param InterfaceType
     * @param fileName
     * @param section
     * @return AbstractNetworkTransport
     */
static AbstractNetworkTransport *getInstance(QString InterfaceType, QString fileName, QString section);
    /**
     * @brief
     *
     * @param fileName
     * @param section
     * @return AbstractNetworkTransport
     */
static AbstractNetworkTransport *getInstance(QString fileName, QString section);

signals:

};

#endif // ABSTRACTNETWORKTRANSPORTFACTORY_H
