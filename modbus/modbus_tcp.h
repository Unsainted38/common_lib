#ifndef MODBUS_TCP_H
#define MODBUS_TCP_H

#include <QObject>
#include <modbus/abstract_modbus_protocol.h>

/**
 * @brief
 *
 */
class ModBusTcp : public AbstractModBusProtocol
{
    Q_OBJECT
    quint16 transactionID = 0; /**< TODO: describe */
    quint16 expectedTransactionID = 0; /**< TODO: describe */
    bool hasExpectedTransactionID = false; /**< TODO: describe */
    const quint16 protocolID = 0; /**< TODO: describe */
    quint8 slaveID; /**< TODO: describe */
public:
    /**
     * @brief
     *
     * @param slaveID
     */
explicit ModBusTcp(quint16 slaveID);
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

#endif // MODBUS_TCP_H
