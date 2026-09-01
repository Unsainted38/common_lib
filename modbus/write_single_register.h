#ifndef WRITE_SINGLE_REGISTER_H
#define WRITE_SINGLE_REGISTER_H

#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>



/**
 * @brief
 *
 */
class WriteSingleRegister : public AbstractCommand
{
    Q_OBJECT
    AbstractModBusProtocol *protocol; /**< TODO: describe */
    QByteArray cachedCommand; /**< TODO: describe */
    QByteArray cachedPdu; /**< TODO: describe */
    quint16 registerAddress; /**< TODO: describe */
    quint16 reg = 0; /**< TODO: describe */
    quint16 cachedReg = 0; /**< TODO: describe */
    QByteArray buffer; /**< TODO: describe */
    bool commandStatus = false; /**< TODO: describe */
    const quint8 cmdID = 0x06; /**< TODO: describe */
public:
    /**
     * @brief
     *
     * @param regAddress
     * @param protocol
     * @param parent
     */
explicit WriteSingleRegister(quint16 regAddress, AbstractModBusProtocol *protocol, QObject *parent = nullptr);

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

#endif // WRITE_SINGLE_REGISTER_H
