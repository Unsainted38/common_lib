#ifndef WRITE_MULTIPLE_COILS_H
#define WRITE_MULTIPLE_COILS_H

#include <QObject>
#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>


/**
 * @brief
 *
 */
class WriteMultipleCoils : public AbstractCommand
{
    Q_OBJECT
    AbstractModBusProtocol *protocol; /**< TODO: describe */
    QByteArray cachedCommand; /**< TODO: describe */
    QByteArray cachedPdu; /**< TODO: describe */
    quint16 coilAddress; /**< TODO: describe */
    quint16 coilsCount; /**< TODO: describe */
    QVector<quint8> coils; /**< TODO: describe */
    QVector<quint8> cachedCoils; /**< TODO: describe */
    QByteArray buffer; /**< TODO: describe */
    bool commandStatus = false; /**< TODO: describe */
    const quint8 cmdID = 0x0F; /**< TODO: describe */
public:
    /**
     * @brief
     *
     * @param coilAddress
     * @param coilsCount
     * @param protocol
     * @param parent
     */
explicit WriteMultipleCoils(quint16 coilAddress, quint16 coilsCount, AbstractModBusProtocol *protocol, QObject *parent = nullptr);

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

#endif // WRITE_MULTIPLE_COILS_H
