#ifndef COMPASSLCC5000COMMAND_H
#define COMPASSLCC5000COMMAND_H

#include "abstract_command.h"
#include "algorithm/crc8.h"
#include "algorithm/binary_coded_decimals_converter.h"

const quint8 HEADERER = 0x77;

class CompassLCC5000Command : public AbstractCommand {
    quint8 m_deviceAddr;
    quint8 m_cmdId;
    quint8 m_length;
    QByteArray cachedRead;
    QByteArray cachedWrite;
public:
    explicit CompassLCC5000Command(quint8 deviceAddr, quint8 cmdId, quint8 length, ValueType value_type, CommandType cmd_type);
    const QByteArray &makeReadCommand();
    const QByteArray &makeWriteCommand();

    // AbstractCommand interface
public:
    const QByteArray &makeCommand() override;
};

#endif // COMPASSLCC5000COMMAND_H
