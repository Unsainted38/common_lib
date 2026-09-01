#ifndef ABSTRACT_MODBUS_PROTOCOL_H
#define ABSTRACT_MODBUS_PROTOCOL_H

#include <QObject>
#include <QByteArray>

/**
 * @brief
 *
 */
enum class ModbusParseStatus {
    Incomplete, // Нужно больше данных
    Invalid,    // Кадр повреждён и удалён из буфера
    Complete    // Получен корректный кадр
};

/**
 * @brief
 *
 */
struct ModbusFrame {
    quint16 transactionId = 0; // Для RTU всегда 0 /**< TODO: describe */
    quint8 deviceId = 0; /**< TODO: describe */
    QByteArray pdu;            // function code + response data /**< TODO: describe */
};

/**
 * @brief
 *
 */
class AbstractModBusProtocol : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
explicit AbstractModBusProtocol(QObject *parent = nullptr)
        : QObject(parent)
    {}
    /**
     * @brief
     *
     * @param pdu
     * @return QByteArray
     */
virtual QByteArray pack(const QByteArray &pdu) = 0;
    /**
     * @brief
     *
     * @param buffer
     * @param frame
     * @return ModbusParseStatus
     */
virtual ModbusParseStatus tryExtractFrame(QByteArray &buffer, ModbusFrame &frame) = 0;
    /**
     * @brief
     *
     * @return quint8
     */
virtual quint8 deviceID() = 0;
signals:
};

#endif // ABSTRACT_MODBUS_PROTOCOL_H
