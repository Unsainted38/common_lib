#include "compass_lcc5000_parser.h"
const quint8 PACKET_BEGIN = 0x77;

CompassLCC5000Parser::CompassLCC5000Parser(QObject *parent)
    : QObject(parent) {

}

CompassLCC5000Parser::CompassLCC5000Parser(quint8 deviceAddr, QObject *parent)
    : QObject(parent),
      m_deviceAddr(deviceAddr) {

}

bool CompassLCC5000Parser::parseReply(const QByteArray &reply) {
    m_buffer.append(reply);
    int beginIndex = m_buffer.indexOf(PACKET_BEGIN);


    if(beginIndex < 0) {
        return false;
    }

    m_buffer.remove(0, beginIndex);
    beginIndex = 0;

    if(m_buffer.size() < 2) {
        return false;
    }

    quint8 packetLength = m_buffer[1];

    if(m_buffer.size() < packetLength + 1) {
        return false;
    }

    QByteArray packet = m_buffer.mid(beginIndex + 1, packetLength);
    quint8 calculatedCrc8 = BytesSumCrc(packet.mid(0, packetLength - 1));
    QDataStream i(&packet, QIODevice::ReadOnly);
    i.setByteOrder(QDataStream::BigEndian);
    quint8 addr;
    quint8 cmdId;
    QByteArray data;
    i >> packetLength
      >> addr
      >> cmdId;


    data = packet.mid(3, packetLength - 4);
    quint8 crc8 = packet[packetLength - 1];

    m_buffer.remove(0, packetLength + 1);

    if(m_deviceAddr != addr) {
        return false;
    }

    emit lastAnswer(packet);

    if(crc8 == calculatedCrc8) {
        emit dataReady(data, cmdId);
        return true;
    }

    qDebug() << "CompassLCC5000 address:" << addr << "checksum error!";
    return false;
}
