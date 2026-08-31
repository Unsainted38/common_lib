#ifndef MODBUS_TCP_H
#define MODBUS_TCP_H

#include <QObject>
#include <modbus/abstract_modbus_protocol.h>

class ModBusTcp : public AbstractModBusProtocol
{
    Q_OBJECT
    quint16 transactionID = 0;
    quint16 expectedTransactionID = 0;
    bool hasExpectedTransactionID = false;
    const quint16 protocolID = 0;
    quint8 slaveID;
public:
    explicit ModBusTcp(quint16 slaveID);
    QByteArray pack(const QByteArray &pdu) override;
    quint8 deviceID() override;
    ModbusParseStatus tryExtractFrame(QByteArray &buffer, ModbusFrame &frame) override;
};

#endif // MODBUS_TCP_H
