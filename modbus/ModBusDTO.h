#ifndef MODBUSDTO_H
#define MODBUSDTO_H

#include <QObject>
#include <modbus/ModBusCmdTypes.h>

/**
 * @brief Содержит поля ответа Modbus для обмена между компонентами.
 */
struct ModBusResponseDto {
    quint8 deviceAddress; /**< Адрес device address. */
    ModBusCmdTypes cmdType; /**< Тип операции команды. */
    quint8 byteCount; /**< Ожидаемое количество байтов данных в ответе. */
    QByteArray data; /**< Текущее значение или полезная нагрузка. */
    quint16 crc16; /**< Хранит crc16. */
};

/**
 * @brief Содержит параметры запроса Modbus.
 */
struct ModBusRequestDto {
    quint8 deviceAddress; /**< Адрес device address. */
    ModBusCmdTypes cmdType; /**< Тип операции команды. */
    quint16 regAddress; /**< Адрес reg address. */
    quint16 regsCount; /**< Количество regs . */
    quint8 byteCount; /**< Ожидаемое количество байтов данных в ответе. */
    QByteArray data; /**< Текущее значение или полезная нагрузка. */
    quint16 crc16; /**< Хранит crc16. */
};

#endif // MODBUSDTO_H
