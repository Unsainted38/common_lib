#ifndef ABSTRACT_MODBUS_PROTOCOL_H
#define ABSTRACT_MODBUS_PROTOCOL_H

#include <QObject>
#include <QByteArray>
#include <algorithm/crc16.h>

class AbstractModBusProtocol : public QObject
{
    Q_OBJECT
public:
    explicit AbstractModBusProtocol(QObject *parent = nullptr);
    virtual QByteArray pack(QByteArray &pdu) = 0;
    virtual quint8 deviceID() = 0;
signals:
};

#endif // ABSTRACT_MODBUS_PROTOCOL_H
