#include "um2_13_commnad.h"

UM2_13Commnad::UM2_13Commnad(quint16 cmdId, quint16 length, ValueType valueType, CommandType cmdType)
    :
    cmdId(cmdId),
    length(length),
    cmdType(cmdType),
    valueType(valueType)
{}


const QByteArray &UM2_13Commnad::makeCommand()
{
    if (!cachedCommand.isEmpty() && value == cachedValue) {
        return cachedCommand;
    }
    cachedValue = value;
    QDataStream out(&cachedCommand, QDataStream::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out << cmdId;
    if (cmdType == CommandType::WRITE){
        switch (valueType) {
        case ValueType::QUINT8:
            out << value.value<quint8>();
            break;
        case ValueType::QUINT16:
            out << value.value<quint16>();
            break;
        default:
            break;
    }
    }
    return cachedCommand;
}

QVariant UM2_13Commnad::getValue()
{
    return value;
}

void UM2_13Commnad::setValue(QVariant v)
{
    value = v;
}

bool UM2_13Commnad::tryParse(const QByteArray &data)
{
    quint16 CmdId;
    buffer.append(data);
    QDataStream in(&buffer, QIODevice::ReadOnly);
    in.setByteOrder(QDataStream::LittleEndian);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);
    in >> CmdId;
    if (CmdId == cmdId && length == buffer.size()) {
        QByteArray cargo;
        while(!in.atEnd()) {
            quint8 b;
            in >> b;
            cargo.append(b);
        }
        value = QVariant::fromValue(cargo);
        emit translateLastAnswer(buffer);
        buffer.clear();
        return true;
    }
    if (buffer.size() >= 1024) buffer.clear();
    return false;
}
