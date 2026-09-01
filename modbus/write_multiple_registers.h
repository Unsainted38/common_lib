#ifndef WRITE_MULTIPLE_REGISTERS_H
#define WRITE_MULTIPLE_REGISTERS_H

#include <QObject>
#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>


/**
 * @brief
 *
 */
class WriteMultipleRegisters : public AbstractCommand
{
    AbstractModBusProtocol *protocol; /**< TODO: describe */
    QByteArray cachedCommand; /**< TODO: describe */
    QByteArray cachedPdu; /**< TODO: describe */
    quint16 registerAddress; /**< TODO: describe */
    quint16 registersCount; /**< TODO: describe */
    QByteArray buffer; /**< TODO: describe */
    QVector<quint16> regs; /**< TODO: describe */
    QVector<quint16> cachedRegs; /**< TODO: describe */
    bool commandStatus = false; /**< TODO: describe */
    const quint8 cmdID = 0x10; /**< TODO: describe */
public:
    /**
     * @brief
     *
     * @param regAddress
     * @param regsCount
     * @param protocol
     * @param parent
     */
explicit WriteMultipleRegisters(quint16 regAddress, quint16 regsCount, AbstractModBusProtocol *protocol, QObject *parent = nullptr);

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
     * @param v
     */
void setValue(QVariant v) override;
    /**
     * @brief
     *
     * @return bool
     */
bool isSuccess() override;
    /**
     * @brief
     *
     * @param data
     * @return bool
     */
bool tryParse(const QByteArray &data) override;
};

#endif // WRITE_MULTIPLE_REGISTERS_H
