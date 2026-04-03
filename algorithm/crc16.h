#ifndef CRC16_H
#define CRC16_H

#include <QtCore>
#include <QtGlobal>

quint16 GetCrc16(const QByteArray &array);
quint16 GetCrc16_n(const QByteArray &array);
quint16 GetCrc16_ubpch(const QByteArray &array);


#endif // CRC16_H
