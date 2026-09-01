#ifndef CRC8_H
#define CRC8_H
#include <QtCore>
#include <QtGlobal>

/**
 * @brief Вычисляет восьмибитную сумму всех байтов массива.
 *
 * @param data Входные данные или полезная нагрузка ответа.
 * @return Результат операции типа quint8.
 */
quint8 BytesSumCrc(const QByteArray data);

#endif // CRC8_H
