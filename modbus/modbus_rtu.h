#ifndef MODBUS_RTU_H
#define MODBUS_RTU_H

#include <modbus/abstract_modbus_protocol.h>
#include <algorithm/crc16.h>
#include <utilities/bit_utils.h>

class ModBusRtu : public AbstractModBusProtocol
{
    Q_OBJECT
    quint8 slaveID;
public:
    explicit ModBusRtu(quint8 slaveID);
    QByteArray pack(const QByteArray &pdu) override;
    quint8 deviceID() override;
    ModbusParseStatus tryExtractFrame(QByteArray &buffer, ModbusFrame &frame) override;
};

#endif // MODBUS_RTU_H
