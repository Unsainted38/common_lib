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
    buffer.clear();

    if (cmdType != CommandType::READ && cmdType != CommandType::WRITE) {
        qWarning() << "Unsupported UM2 command type:"
                   << static_cast<int>(cmdType);
        cachedCommand.clear();
        return cachedCommand;
    }

    if (!cachedCommand.isEmpty() && value == cachedValue) {
        return cachedCommand;
    }
    cachedValue = value;
    cachedCommand.clear();
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
            qWarning() << "Unsupported UM2 value type:" << valueType;
            cachedCommand.clear();
            return cachedCommand;
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
    buffer.append(data);

    if (length < sizeof(quint16)) {
        buffer.clear();
        return false;
    }

    QByteArray commandMarker;
    commandMarker.append(static_cast<char>(cmdId & 0xFF));
    commandMarker.append(static_cast<char>((cmdId >> 8) & 0xFF));

    while (true) {
        const qsizetype markerIndex = buffer.indexOf(commandMarker);
        if (markerIndex < 0) {
            if (!buffer.isEmpty() && buffer.back() == commandMarker.front()) {
                buffer = buffer.last(1);
            } else {
                buffer.clear();
            }
            return false;
        }

        buffer.remove(0, markerIndex);
        if (buffer.size() < length) {
            return false;
        }

        const QByteArray frame = buffer.first(length);
        buffer.remove(0, length);

        QDataStream in(frame);
        in.setByteOrder(QDataStream::LittleEndian);
        in.setFloatingPointPrecision(QDataStream::SinglePrecision);

        quint16 responseCommand = 0;
        in >> responseCommand;
        if (responseCommand != cmdId || in.status() != QDataStream::Ok) {
            continue;
        }

        const QByteArray cargo = frame.sliced(sizeof(quint16));
        switch (valueType) {
        case ValueType::QUINT8: {
            if (cargo.size() < static_cast<qsizetype>(sizeof(quint8))) {
                continue;
            }
            value = static_cast<quint8>(cargo[0]);
            break;
        }
        case ValueType::QUINT16: {
            if (cargo.size() < static_cast<qsizetype>(sizeof(quint16))) {
                continue;
            }
            QDataStream cargoStream(cargo);
            cargoStream.setByteOrder(QDataStream::LittleEndian);
            quint16 parsedValue = 0;
            cargoStream >> parsedValue;
            if (cargoStream.status() != QDataStream::Ok) {
                continue;
            }
            value = parsedValue;
            break;
        }
        case ValueType::QBYTEARRAY:
            value = cargo;
            break;
        default:
            qWarning() << "Unsupported UM2 response value type:" << valueType;
            continue;
        }

        emit translateLastAnswer(frame);
        return true;
    }
}
