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
    }

    o << GetCrc16_ubpch(res);

    res = pack(res);
    cachedWrite = res;
    return cachedWrite;
}

const QByteArray &UBPChCommand::makeCommand()
{
    switch (cmdType) {
    case CommandType::READ:
        return makeReadCommand();
    case CommandType::WRITE:
        return makeWriteCommand();
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

    switch(valueType) {
        case ValueType::QUINT8:
            quint8 valu8;
            in >> valu8;
            value = QVariant::fromValue(valu8);
            break;

        case ValueType::QINT8:
            qint8 val8;
            in >> val8;
            value = QVariant::fromValue(val8);
            break;

        case ValueType::QUINT16:
            quint16 valu16;
            in >> valu16;
            value = QVariant::fromValue(valu16);
            break;

        case ValueType::QUINT32:
            quint32 valu32;
            in >> valu32;
            value = QVariant::fromValue(valu32);
            break;

        case ValueType::QINT32:
            quint32 val32;
            in >> val32;
            value = QVariant::fromValue(val32);
            break;
    };
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
