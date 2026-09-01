#ifndef ABSTRACT_MODBUS_PROTOCOL_H
#define ABSTRACT_MODBUS_PROTOCOL_H

#include <QObject>
#include <QByteArray>

/**
 * @brief Описывает результат попытки извлечения кадра из потокового буфера.
 */
enum class ModbusParseStatus {
    Incomplete, // Нужно больше данных
    Invalid,    // Кадр повреждён и удалён из буфера
    Complete    // Получен корректный кадр
};

/**
 * @brief Содержит разобранные поля кадра Modbus и его PDU.
 */
struct ModbusFrame {
    quint16 transactionId = 0; /**< Идентификатор транзакции; для RTU всегда равен 0. */
    quint8 deviceId = 0; /**< Хранит device id. */
    QByteArray pdu; /**< PDU: код функции и данные ответа. */
};

/**
 * @brief Определяет упаковку PDU и потоковое извлечение кадров Modbus.
 */
class AbstractModBusProtocol : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Определяет упаковку PDU и потоковое извлечение кадров Modbus.
     *
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit AbstractModBusProtocol(QObject *parent = nullptr)
        : QObject(parent)
    {}
    /**
     * @brief Упаковывает полезную нагрузку в кадр соответствующего протокола.
     *
     * @param pdu Блок данных протокола Modbus без транспортного заголовка.
     * @return Сформированный массив байтов.
     */
virtual QByteArray pack(const QByteArray &pdu) = 0;
    /**
     * @brief Пытается извлечь один полный кадр из накопительного буфера.
     *
     * @param buffer Накопительный буфер; обработанные байты удаляются.
     * @param frame Структура, в которую записывается разобранный кадр.
     * @return Статус разбора: неполный, некорректный или завершённый кадр.
     */
virtual ModbusParseStatus tryExtractFrame(QByteArray &buffer, ModbusFrame &frame) = 0;
    /**
     * @brief Возвращает адрес ведомого Modbus-устройства.
     *
     * @return Адрес устройства.
     */
virtual quint8 deviceID() = 0;
signals:
};

#endif // ABSTRACT_MODBUS_PROTOCOL_H
