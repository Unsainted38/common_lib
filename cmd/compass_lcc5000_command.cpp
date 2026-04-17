#include "compass_lcc5000_command.h"

CompassLCC5000Command::CompassLCC5000Command(quint8 deviceAddr, quint8 cmdId, quint8 length, ValueType valueType, CommandType cmdType)
    : m_deviceAddr(deviceAddr),
      m_cmdId(cmdId),
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

    switch(valueType) {
        case ValueType::QUINT8:
            for(auto b : BcdConverter::fromQuint8_XX(data.toUInt())) {
                o << static_cast<quint8>(b);
            }

            break;

        case ValueType::FLOAT:
            for(auto b : BcdConverter::fromFloat_SXXY(data.toFloat())) {
                o << static_cast<quint8>(b);
            }

            break;

        case ValueType::DOUBLE:
            for(auto b : BcdConverter::fromDouble_SXXY(data.toDouble())) {
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

const QByteArray &CompassLCC5000Command::makeCommand()
{
    switch (cmdType) {
    case CommandType::READ:
        return makeReadCommand();
    case CommandType::WRITE:
        return makeWriteCommand();
    }
}

QVariant CompassLCC5000Command::getValue()
{
    return data;
}

void CompassLCC5000Command::setValue(QVariant v)
{
    data = v;
}
