#ifndef CRC16_H
#define CRC16_H

#include <QtCore>
#include <QtGlobal>

/**
 * @brief
 *
 * @param array
 * @return quint16
 */
quint16 GetCrc16(const QByteArray &array);
/**
 * @brief
 *
 * @param array
 * @return quint16
 */
quint16 GetCrc16_n(const QByteArray &array);
/**
 * @brief
 *
 * @param array
 * @return quint16
 */
quint16 GetCrc16_ubpch(const QByteArray &array);


#endif // CRC16_H
