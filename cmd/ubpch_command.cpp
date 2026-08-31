#include "ubpch_command.h"
#include "crc16.h"




const QByteArray &UBPChCommand::makeReadCommand() {
    if (cachedRead.isEmpty()) {
        QByteArray res;
        QDataStream o(&res, QIODevice::ReadWrite);
        o.setByteOrder(QDataStream::LittleEndian);
        o.setFloatingPointPrecision(QDataStream::SinglePrecision);
        o << m_deviceAddr
          << m_masterAddr
          << readCmd
          << m_regAddr
          << GetCrc16_ubpch(res);

        cachedRead = pack(res);

    }
    return cachedRead;
}

const QByteArray &UBPChCommand::makeWriteCommand() {
    QByteArray res;
    QDataStream o(&res, QIODevice::ReadWrite);
    o.setByteOrder(QDataStream::LittleEndian);
    o.setFloatingPointPrecision(QDataStream::SinglePrecision);
    o << m_deviceAddr
      << m_masterAddr
      << writeCmd
      << m_regAddr;

    switch(valueType) {
        case ValueType::QINT8:
            o << (qint8)value.toInt();
            break;

        case ValueType::QUINT8:
            o << (quint8)value.toUInt();
            break;

        case ValueType::QUINT16:
            o << (quint16)value.toUInt();
            break;

        case ValueType::QUINT32:
            o << (quint32)value.toUInt();
            break;

        case ValueType::QINT32:
            o << (qint32)value.toInt();
            break;
        default:
            qDebug() << "Unsupported value type!";
            cachedWrite.clear();
            return cachedWrite;
    }

    o << GetCrc16_ubpch(res);

    res = pack(res);
    cachedWrite = res;
    return cachedWrite;
}

const QByteArray &UBPChCommand::makeCommand()
{
    responseBuffer.clear();

    switch (cmdType) {
    case CommandType::READ:
        return makeReadCommand();
    case CommandType::WRITE:
        return makeWriteCommand();
    default:
        qWarning() << "Unsupported UBPCh command type:"
                   << static_cast<int>(cmdType);
        cachedRead.clear();
        return cachedRead;
    }
}

bool UBPChCommand::tryParse(const QByteArray &reply)
{
    constexpr qsizetype markerSize = 2;
    constexpr qsizetype trailerSize = 4;
    constexpr qsizetype minimumPacketSize = 11;
    const QByteArray header = QByteArray::fromHex("FE FE");
    const QByteArray end = QByteArray::fromHex("FC FC");

    responseBuffer.append(reply);
    while (true) {
        const qsizetype headerIndex = responseBuffer.indexOf(header);
        if (headerIndex < 0) {
            if (!responseBuffer.isEmpty() &&
                static_cast<quint8>(responseBuffer.back()) == 0xFE) {
                responseBuffer = responseBuffer.last(1);
            } else {
                responseBuffer.clear();
            }
            return false;
        }

        responseBuffer.remove(0, headerIndex);
        const qsizetype endIndex =
            responseBuffer.indexOf(end, header.size());
        if (endIndex < 0) {
            return false;
        }

        const QByteArray packet = unpack(
            responseBuffer.first(endIndex + end.size()));
        responseBuffer.remove(0, endIndex + end.size());
        if (packet.size() < minimumPacketSize) {
            continue;
        }

        quint16 start = 0;
        quint8 destination = 0;
        quint8 source = 0;
        quint8 command = 0;
        quint16 registerAddress = 0;
        quint16 receivedCrc = 0;
        quint16 stop = 0;

        QDataStream in(packet);
        in.setByteOrder(QDataStream::LittleEndian);
        in >> start >> destination >> source >> command >> registerAddress;
        in.device()->seek(packet.size() - trailerSize);
        in >> receivedCrc >> stop;

        const QByteArray crcPayload = packet.mid(
            markerSize, packet.size() - markerSize - trailerSize);
        const bool expectedCommand =
            command == 0x0A ||
            (cmdType == CommandType::READ && command == 0x04) ||
            (cmdType == CommandType::WRITE && command == 0x06);

        if (in.status() == QDataStream::Ok &&
            start == HEADER &&
            stop == END &&
            destination == m_masterAddr &&
            source == m_deviceAddr &&
            expectedCommand &&
            (command == 0x0A || registerAddress == m_regAddr) &&
            receivedCrc == GetCrc16_ubpch(crcPayload)) {
            return true;
        }
    }
}

QVariant UBPChCommand::getValue()
{
    return value;
}

void UBPChCommand::setValue(QVariant v)
{
    value = v;
}

void UBPChCommand::processData(const QByteArray &data, quint16 regAddr) {
    if(m_regAddr != regAddr) {
        return;
    }

    QDataStream in(data);
    in.setByteOrder(QDataStream::LittleEndian);

    QVariant parsedValue;
    switch(valueType) {
        case ValueType::QUINT8: {
            quint8 value8 = 0;
            in >> value8;
            parsedValue = value8;
            break;
        }

        case ValueType::QINT8: {
            qint8 value8 = 0;
            in >> value8;
            parsedValue = value8;
            break;
        }

        case ValueType::QUINT16: {
            quint16 value16 = 0;
            in >> value16;
            parsedValue = value16;
            break;
        }

        case ValueType::QUINT32: {
            quint32 value32 = 0;
            in >> value32;
            parsedValue = value32;
            break;
        }

        case ValueType::QINT32: {
            qint32 value32 = 0;
            in >> value32;
            parsedValue = value32;
            break;
        }
        default:
            qWarning() << "Unsupported UBPCh response value type:"
                       << valueType;
            break;
    };

    if (in.status() == QDataStream::Ok && parsedValue.isValid()) {
        value = parsedValue;
    } else {
        qWarning() << "Invalid UBPCh response payload for register:"
                   << regAddr;
    }
}

QByteArray UBPChCommand::unpack(const QByteArray &frame)
{
    QByteArray result;
    result.reserve(frame.size());

    for (qsizetype i = 0; i < frame.size(); ++i) {
        const quint8 byte = static_cast<quint8>(frame.at(i));
        if ((byte == 0xFE || byte == 0xFC) &&
            i + 1 < frame.size() && frame.at(i + 1) == 0x00) {
            result.append(static_cast<char>(byte));
            ++i;
        } else {
            result.append(static_cast<char>(byte));
        }
    }

    return result;
}
QByteArray UBPChCommand::pack(const QByteArray &frame) {
    QByteArray result;
    result.reserve(frame.size() * 2);

    result.append(static_cast<char>(0xFE));
    result.append(static_cast<char>(0xFE));

    for(int i = 0; i < frame.size(); ++i) {
        quint8 byte = static_cast<quint8>(frame[i]);
        result.append(static_cast<char>(byte));

        if(byte == 0xFE || byte == 0xFC) {
            result.append(static_cast<char>(0x00));
        }
    }

    result.append(static_cast<char>(0xFC));
    result.append(static_cast<char>(0xFC));

    return result;
}
