#include "compass_lcc5000_command.h"

CompassLCC5000Command::CompassLCC5000Command(quint8 deviceAddr, quint8 cmdId, quint8 length, ValueType type)
    : AbstractCommand(type),
      m_cmdId(cmdId),
      m_deviceAddr(deviceAddr),
      m_length(length) {

}

const QByteArray &CompassLCC5000Command::makeReadCommand() {
    if (cachedRead.isEmpty()) {
        QByteArray res;
        QDataStream o(&res, QIODevice::WriteOnly);
        o.setByteOrder(QDataStream::BigEndian);
        o << m_length
          << m_deviceAddr
          << m_cmdId;
        quint8 crc8 = BytesSumCrc(res);
        o << crc8;
        res.push_front(HEADERER);
        cachedRead = res;
    }
    return cachedRead;
}

const QByteArray &CompassLCC5000Command::makeWriteCommand() {
    QByteArray res;
    QDataStream o(&res, QIODevice::WriteOnly);
    o.setByteOrder(QDataStream::BigEndian);
    o << m_length
      << m_deviceAddr
      << m_cmdId;

    switch(m_type) {
        case ValueType::QUINT8:
            for(auto b : BcdConverter::fromQuint8_XX(m_valu8)) {
                o << static_cast<quint8>(b);
            }

            break;

        case ValueType::FLOAT:
            for(auto b : BcdConverter::fromFloat_SXXY(m_valf)) {
                o << static_cast<quint8>(b);
            }

            break;

        case ValueType::DOUBLE:
            for(auto b : BcdConverter::fromDouble_SXXY(m_vald)) {
                o << static_cast<quint8>(b);
            }

            break;
    }

    quint8 crc8 = BytesSumCrc(res);
    o  << crc8;
    res.push_front(HEADERER);
    cachedWrite = res;
    return cachedWrite;
}
