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

    switch(m_type) {
        case ValueType::QINT8:
            o << m_val8;
            break;

        case ValueType::QUINT8:
            o << m_valu8;
            break;

        case ValueType::QUINT16:
            o << m_valu16;
            break;

        case ValueType::QUINT32:
            o << m_valu32;
            break;

        case ValueType::QINT32:
            o << m_val32;
            break;
    }

    o << GetCrc16_ubpch(res);

    res = pack(res);
    cachedWrite = res;
    return cachedWrite;
}

void UBPChCommand::processData(const QByteArray &data, quint16 regAddr) {
    if(m_regAddr != regAddr) {
        return;
    }

    QDataStream in(data);
    in.setByteOrder(QDataStream::LittleEndian);

    switch(m_type) {
        case ValueType::QUINT8:
            in >> m_valu8;
            break;

        case ValueType::QINT8:
            in >> m_val8;

        case ValueType::QUINT16:
            in >> m_valu16;
            break;

        case ValueType::QUINT32:
            in >> m_valu32;
            break;

        case ValueType::QINT32:
            in >> m_val32;
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
