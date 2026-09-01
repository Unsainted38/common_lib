#ifndef MODBUS_TCP_H
#define MODBUS_TCP_H

#include <QObject>
#include <modbus/abstract_modbus_protocol.h>

/**
 * @brief Упаковывает и извлекает Modbus TCP ADU с проверкой MBAP и transaction ID.
 */
class ModBusTcp : public AbstractModBusProtocol
{
    Q_OBJECT
    quint16 transactionID = 0; /**< Идентификатор, назначаемый следующему Modbus TCP-запросу. */
    quint16 expectedTransactionID = 0; /**< Идентификатор ожидаемого Modbus TCP-ответа. */
    bool hasExpectedTransactionID = false; /**< Показывает, задан ли ожидаемый transaction ID. */
    const quint16 protocolID = 0; /**< Идентификатор протокола MBAP; для Modbus равен нулю. */
    quint8 slaveID; /**< Адрес ведомого Modbus-устройства. */
public:
    /**
     * @brief Упаковывает и извлекает Modbus TCP ADU с проверкой MBAP и transaction ID.
     *
     * @param slaveID Адрес ведомого Modbus-устройства.
     */
explicit ModBusTcp(quint16 slaveID);
    /**
     * @brief Упаковывает полезную нагрузку в кадр соответствующего протокола.
     *
     * @param pdu Блок данных протокола Modbus без транспортного заголовка.
     * @return Сформированный массив байтов.
     */
QByteArray pack(const QByteArray &pdu) override;
    /**
     * @brief Возвращает адрес ведомого Modbus-устройства.
     *
     * @return Адрес устройства.
     */
quint8 deviceID() override;
    /**
     * @brief Пытается извлечь один полный кадр из накопительного буфера.
     *
     * @param buffer Накопительный буфер; обработанные байты удаляются.
     * @param frame Структура, в которую записывается разобранный кадр.
     * @return Статус разбора: неполный, некорректный или завершённый кадр.
     */
ModbusParseStatus tryExtractFrame(QByteArray &buffer, ModbusFrame &frame) override;
};

#endif // MODBUS_TCP_H
