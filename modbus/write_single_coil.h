#ifndef WRITE_SINGLE_COIL_H
#define WRITE_SINGLE_COIL_H

#include <QObject>
#include <QVector>
#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>


/**
 * @brief
 *
 */
class WriteSingleCoil : public AbstractCommand
{
    Q_OBJECT
    bool coil = false; /**< TODO: describe */
    bool cachedCoil = false; /**< TODO: describe */
    QByteArray cachedCommand; /**< TODO: describe */
    QByteArray cachedPdu; /**< TODO: describe */
    AbstractModBusProtocol *protocol; /**< TODO: describe */
    quint16 coilAddress; /**< TODO: describe */
    QByteArray buffer; /**< TODO: describe */
    bool commandStatus = false; /**< TODO: describe */
    const quint8 cmdID = 0x05; /**< TODO: describe */
public:
    /**
     * @brief
     *
     * @param coilAddress
     * @param protocol
     * @param parent
     */
explicit WriteSingleCoil(quint16 coilAddress, AbstractModBusProtocol *protocol, QObject *parent = nullptr);

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
     * @param boolean
     */
void setValue(QVariant boolean) override;
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

#endif // WRITE_SINGLE_COIL_H
