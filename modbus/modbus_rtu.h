#ifndef MODBUS_RTU_H
#define MODBUS_RTU_H

#include <modbus/abstract_modbus_protocol.h>
#include <algorithm/crc16.h>
#include <utilities/bit_utils.h>

/**
 * @brief
 *
 */
class ModBusRtu : public AbstractModBusProtocol
{
    Q_OBJECT
    quint8 slaveID; /**< TODO: describe */
public:
    /**
     * @brief
     *
     * @param slaveID
     */
explicit ModBusRtu(quint8 slaveID);
    /**
     * @brief
     *
     * @param pdu
     * @return QByteArray
     */
QByteArray pack(const QByteArray &pdu) override;
    /**
     * @brief
     *
     * @return quint8
     */
quint8 deviceID() override;
    /**
     * @brief
     *
     * @param buffer
     * @param frame
     * @return ModbusParseStatus
     */
ModbusParseStatus tryExtractFrame(QByteArray &buffer, ModbusFrame &frame) override;
};

#endif // MODBUS_RTU_H
