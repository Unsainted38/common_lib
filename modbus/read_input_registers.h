#ifndef READ_INPUT_REGISTERS_H
#define READ_INPUT_REGISTERS_H

#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>


/**
 * @brief
 *
 */
class ReadInputRegisters : public AbstractCommand
{
    Q_OBJECT
    QByteArray cachedCommand; /**< TODO: describe */
    QByteArray cachedPdu; /**< TODO: describe */
    AbstractModBusProtocol *protocol; /**< TODO: describe */
    quint16 registerAddress; /**< TODO: describe */
    quint16 registersCount; /**< TODO: describe */
    quint8 byteCount; /**< TODO: describe */
    QByteArray buffer; /**< TODO: describe */
    QVector<quint16> regs; /**< TODO: describe */
    const quint8 cmdID = 0x04; /**< TODO: describe */

public:
    /**
     * @brief
     *
     * @param regAddress
     * @param regsCount
     * @param protocol
     * @param parent
     */
explicit ReadInputRegisters(quint16 regAddress, quint16 regsCount, AbstractModBusProtocol *protocol, QObject *parent = nullptr);

    // AbstractCommand interface
public:
    /**
     * @brief
     *
     * @return const QByteArray
     */
const QByteArray &makeCommand() override;
    /**
     * @brief
     *
     * @return QVariant
     */
QVariant getValue() override;
    /**
     * @brief
     *
     * @param data
     * @return bool
     */
bool tryParse(const QByteArray &data) override;
};

#endif // READ_INPUT_REGISTERS_H
