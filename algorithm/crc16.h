#ifndef CRC16_H
#define CRC16_H

#include <QtCore>
#include <QtGlobal>

/**
 * @brief Вычисляет Modbus CRC16 без двух последних байтов принятого кадра.
 *
 * @param array Кадр, два последних байта которого содержат принятый CRC.
 * @return Результат операции типа quint16.
 */
quint16 GetCrc16(const QByteArray &array);
/**
 * @brief Вычисляет Modbus CRC16 по всему переданному массиву.
 *
 * @param array Данные, для которых вычисляется CRC.
 * @return Результат операции типа quint16.
 */
quint16 GetCrc16_n(const QByteArray &array);
/**
 * @brief Вычисляет CRC16 протокола УБПЧ с начальным значением 0x50C0.
 *
 * @param array Неэкранированная полезная нагрузка кадра УБПЧ.
 * @return Результат операции типа quint16.
 */
quint16 GetCrc16_ubpch(const QByteArray &array);


#endif // CRC16_H
