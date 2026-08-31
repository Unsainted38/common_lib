#include "compass_lcc5000_command.h"
#include "algorithm/crc8.h"
#include "algorithm/binary_coded_decimals_converter.h"

CompassLCC5000Command::CompassLCC5000Command(quint8 deviceAddr, quint8 cmdId, quint8 length, ValueType valueType, CommandType cmdType)
    : m_deviceAddr(deviceAddr),
      m_cmdId(cmdId),
      m_length(length),
      valueType(valueType),
      cmdType(cmdType) {

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

        default:
            qWarning() << "Unsupported compass value type:" << valueType;
            cachedWrite.clear();
            return cachedWrite;
    }

    quint8 crc8 = BytesSumCrc(res);
    o  << crc8;
    res.push_front(HEADERER);
    cachedWrite = res;
    return cachedWrite;
}

const QByteArray &CompassLCC5000Command::makeCommand()
{
    responseBuffer.clear();

    switch (cmdType) {
    case CommandType::READ:
        return makeReadCommand();
    case CommandType::WRITE:
        return makeWriteCommand();
    default:
        qWarning() << "Unsupported compass command type:"
                   << static_cast<int>(cmdType);
        cachedRead.clear();
        return cachedRead;
    }
}

bool CompassLCC5000Command::tryParse(const QByteArray &reply)
{
    responseBuffer.append(reply);

    while (true) {
        const qsizetype beginIndex = responseBuffer.indexOf(HEADERER);
        if (beginIndex < 0) {
            responseBuffer.clear();
            return false;
        }

        responseBuffer.remove(0, beginIndex);
        if (responseBuffer.size() < 2) {
            return false;
        }

        const quint8 packetLength =
            static_cast<quint8>(responseBuffer.at(1));
        if (packetLength < 4) {
            responseBuffer.remove(0, 1);
            continue;
        }

        const qsizetype frameSize =
            static_cast<qsizetype>(packetLength) + 1;
        if (responseBuffer.size() < frameSize) {
            return false;
        }

        const QByteArray packet = responseBuffer.mid(1, packetLength);
        responseBuffer.remove(0, frameSize);

        const quint8 address = static_cast<quint8>(packet.at(1));
        const quint8 responseCommand =
            static_cast<quint8>(packet.at(2));
        const quint8 receivedCrc =
            static_cast<quint8>(packet.at(packetLength - 1));
        const quint8 expectedCommand = m_cmdId | 0x80;

        if (address == m_deviceAddr &&
            (responseCommand == m_cmdId ||
             responseCommand == expectedCommand) &&
            receivedCrc == BytesSumCrc(packet.first(packetLength - 1))) {
            return true;
        }
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
