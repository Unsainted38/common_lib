#ifndef MODBUSDTO_H
#define MODBUSDTO_H

#include <QObject>
#include <modbus/ModBusCmdTypes.h>

/**
 * @brief
 *
 */
struct ModBusResponseDto {
    quint8 deviceAddress; /**< TODO: describe */
    ModBusCmdTypes cmdType; /**< TODO: describe */
    quint8 byteCount; /**< TODO: describe */
    QByteArray data; /**< TODO: describe */
    quint16 crc16; /**< TODO: describe */
};

/**
 * @brief
 *
 */
struct ModBusRequestDto {
    quint8 deviceAddress; /**< TODO: describe */
    ModBusCmdTypes cmdType; /**< TODO: describe */
    quint16 regAddress; /**< TODO: describe */
    quint16 regsCount; /**< TODO: describe */
    quint8 byteCount; /**< TODO: describe */
    QByteArray data; /**< TODO: describe */
    quint16 crc16; /**< TODO: describe */
};

#endif // MODBUSDTO_H
