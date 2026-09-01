#ifndef ABSTRACT_MODBUS_DEVICE_H
#define ABSTRACT_MODBUS_DEVICE_H

#include <QObject>
#include <requesters/serial_circular_requester.h>
#include <utilities/config_helper.h>
#include <modbus/read_holding_registers.h>
#include <modbus/modbus_protocol_factory.h>

/**
 * @brief
 *
 */
class AbstractModbusDevice : public QObject
{
    Q_OBJECT
    quint8 deviceID; /**< TODO: describe */
    QString configPath; /**< TODO: describe */
    QString section; /**< TODO: describe */
    SerialCircularRequester *requester; /**< TODO: describe */
    QByteArray buffer; /**< TODO: describe */

protected:
    AbstractModBusProtocol *protocol; /**< TODO: describe */
    /**
     * @brief
     *
     * @param cmd
     */
void addCircularCommand(AbstractCommand *cmd);
    /**
     * @brief
     *
     * @param cmd
     */
void executeCommand(AbstractCommand *cmd);

public:
    /**
     * @brief
     *
     * @param requester
     * @param configPath
     * @param section
     * @param parent
     */
explicit AbstractModbusDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent = nullptr);
    /**
     * @brief
     *
     * @return quint8
     */
quint8 deviceAddress();

signals:

private slots:
};

#endif // ABSTRACT_MODBUS_DEVICE_H
