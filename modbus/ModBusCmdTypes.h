#ifndef MODBUSCMDTYPES_H
#define MODBUSCMDTYPES_H
#include <QObject>

enum class ModBusCmdTypes : quint8 {
    READ_COILS = 0x01,
    READ_DESCRETE_INPUTS = 0x02,
    READ_HOLDING_REGISTERS = 0x03,
    READ_INPUT_REGISTERS = 0x04,
    WRITE_SINGLE_COIL = 0x05,
    WRITE_SINGLE_REGISTER = 0x06,
    WRITE_MULTIPLE_COILS = 0x0F,
    WRITE_MULTIPLE_REGISTERS = 0x10,
};

constexpr quint8 operator*(ModBusCmdTypes e) {
    return static_cast<quint8>(e);
}

#endif // MODBUSCMDTYPES_H
