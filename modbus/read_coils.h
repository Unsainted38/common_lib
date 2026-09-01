#ifndef READ_COILS_H
#define READ_COILS_H

#include <QObject>
#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>


/**
 * @brief
 *
 */
class ReadCoils : public AbstractCommand
{
    Q_OBJECT
    QByteArray cachedCommand; /**< TODO: describe */
    QByteArray cachedPdu; /**< TODO: describe */
    AbstractModBusProtocol *protocol; /**< TODO: describe */
    quint16 coilAddress; /**< TODO: describe */
    quint16 coilsCount; /**< TODO: describe */
    quint8 byteCount; /**< TODO: describe */
    QByteArray buffer; /**< TODO: describe */
    QVector<quint8> coils; /**< TODO: describe */
    const quint8 cmdID = 0x01; /**< TODO: describe */
public:
    /**
     * @brief
     *
     * @param coilAddress
     * @param coilsCount
     * @param protocol
     * @param parent
     */
explicit ReadCoils(quint16 coilAddress, quint16 coilsCount, AbstractModBusProtocol *protocol, QObject *parent = nullptr);

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


};

#endif // READ_COILS_H
