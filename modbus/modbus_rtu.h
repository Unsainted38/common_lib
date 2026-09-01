#ifndef MODBUS_RTU_H
#define MODBUS_RTU_H

#include <modbus/abstract_modbus_protocol.h>
#include <algorithm/crc16.h>
#include <utilities/bit_utils.h>

/**
 * @brief Упаковывает и извлекает кадры Modbus RTU с проверкой CRC16.
 */
class ModBusRtu : public AbstractModBusProtocol
{
    Q_OBJECT
    quint8 slaveID; /**< Адрес ведомого Modbus-устройства. */
public:
    /**
     * @brief Упаковывает и извлекает кадры Modbus RTU с проверкой CRC16.
     *
     * @param slaveID Адрес ведомого Modbus-устройства.
     */
explicit ModBusRtu(quint8 slaveID);
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

#endif // MODBUS_RTU_H
