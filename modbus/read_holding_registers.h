#ifndef READ_HOLDING_REGISTERS_H
#define READ_HOLDING_REGISTERS_H

#include <QObject>
#include <QVector>
#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>


/**
 * @brief
 *
 */
class ReadHoldingRegisters : public AbstractCommand
{
    QByteArray cachedCommand; /**< TODO: describe */
    QByteArray cachedPdu; /**< TODO: describe */
    AbstractModBusProtocol *protocol; /**< TODO: describe */
    quint16 registerAddress; /**< TODO: describe */
    quint16 registersCount; /**< TODO: describe */
    quint8 byteCount; /**< TODO: describe */
    QByteArray buffer; /**< TODO: describe */
    QVector<quint16> regs; /**< TODO: describe */
    const quint8 cmdID = 0x03; /**< TODO: describe */
public:
    /**
     * @brief
     *
     * @param regAddress
     * @param regsCount
     * @param protocol
     * @param parent
     */
explicit ReadHoldingRegisters(quint16 regAddress, quint16 regsCount, AbstractModBusProtocol *protocol, QObject *parent = nullptr);


    // AbstractCommand interface
public:
    /**
     * @brief
     *
     * @return const QByteArray
     */
const QByteArray &makeCommand() override;

    // AbstractCommand interface
public:
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

public slots:
};

#endif // READ_HOLDING_REGISTERS_H
