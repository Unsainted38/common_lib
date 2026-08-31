#ifndef MODBUS_TCP_H
#define MODBUS_TCP_H

#include <QObject>
#include <modbus/abstract_modbus_protocol.h>

class ModBusTcp : public AbstractModBusProtocol
{
    Q_OBJECT
    quint16 transactionID = 0;
    const quint16 protocolID = 0;
    quint8 slaveID;
public:
    explicit ModBusTcp(quint16 slaveID);
    QByteArray pack(const QByteArray &pdu) override;
signals:


    // AbstractModBusProtocol interface
public:
    quint8 deviceID() override;
};

#endif // MODBUS_TCP_H
