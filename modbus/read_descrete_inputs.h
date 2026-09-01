#ifndef READ_DESCRETE_INPUTS_H
#define READ_DESCRETE_INPUTS_H

#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>


/**
 * @brief
 *
 */
class ReadDescreteInputs : public AbstractCommand
{
    Q_OBJECT
    AbstractModBusProtocol *protocol; /**< TODO: describe */
    QByteArray cachedCommand; /**< TODO: describe */
    QByteArray cachedPdu; /**< TODO: describe */
    quint16 inputAddress; /**< TODO: describe */
    quint16 inputsCount; /**< TODO: describe */
    quint8 byteCount; /**< TODO: describe */
    QByteArray buffer; /**< TODO: describe */
    QVector<quint8> inputs; /**< TODO: describe */
    const quint8 cmdID = 0x02; /**< TODO: describe */
public:
    /**
     * @brief
     *
     * @param inputAddress
     * @param inputsCount
     * @param protocol
     * @param parent
     */
explicit ReadDescreteInputs(quint16 inputAddress, quint16 inputsCount, AbstractModBusProtocol *protocol, QObject *parent = nullptr);

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

#endif // READ_DESCRETE_INPUTS_H
