#ifndef MODBUSDTO_H
#define MODBUSDTO_H

#include <QObject>
#include <modbus/ModBusCmdTypes.h>

struct ModBusResponseDto {
    quint8 deviceAddress;
    ModBusCmdTypes cmdType;
    quint8 byteCount;
    QByteArray data;
    quint16 crc16;
};

struct ModBusRequestDto {
    quint8 deviceAddress;
    ModBusCmdTypes cmdType;
    quint16 regAddress;
    quint16 regsCount;
    quint8 byteCount;
    QByteArray data;
    quint16 crc16;
};

#endif // MODBUSDTO_H
