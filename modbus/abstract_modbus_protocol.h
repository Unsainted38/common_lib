#ifndef ABSTRACT_MODBUS_PROTOCOL_H
#define ABSTRACT_MODBUS_PROTOCOL_H

#include <QObject>
#include <QByteArray>

enum class ModbusParseStatus {
    Incomplete, // Нужно больше данных
    Invalid,    // Кадр повреждён и удалён из буфера
    Complete    // Получен корректный кадр
};

struct ModbusFrame {
    quint16 transactionId = 0; // Для RTU всегда 0
    quint8 deviceId = 0;
    QByteArray pdu;            // function code + response data
};

class AbstractModBusProtocol : public QObject
{
    Q_OBJECT
public:
    explicit AbstractModBusProtocol(QObject *parent = nullptr)
        : QObject(parent)
    {}
    virtual QByteArray pack(const QByteArray &pdu) = 0;
    virtual ModbusParseStatus tryExtractFrame(QByteArray &buffer, ModbusFrame &frame) = 0;
    virtual quint8 deviceID() = 0;
signals:
};

#endif // ABSTRACT_MODBUS_PROTOCOL_H
