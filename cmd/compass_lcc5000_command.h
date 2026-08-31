#ifndef COMPASSLCC5000COMMAND_H
#define COMPASSLCC5000COMMAND_H

#include "abstract_command.h"

const quint8 HEADERER = 0x77;

class CompassLCC5000Command : public AbstractCommand {
    quint8 m_deviceAddr;
    quint8 m_cmdId;
    quint8 m_length;
    ValueType valueType;
    CommandType cmdType;
    QByteArray cachedRead;
    QByteArray cachedWrite;
    QByteArray responseBuffer;
    QVariant data;
public:
    explicit CompassLCC5000Command(quint8 deviceAddr, quint8 cmdId, quint8 length, ValueType valueType, CommandType cmdType);
    const QByteArray &makeReadCommand();
    const QByteArray &makeWriteCommand();

    // AbstractCommand interface
public:
    const QByteArray &makeCommand() override;
    bool tryParse(const QByteArray &data) override;

    // AbstractCommand interface
public:
    QVariant getValue() override;
    void setValue(QVariant v) override;
};

#endif // COMPASSLCC5000COMMAND_H
