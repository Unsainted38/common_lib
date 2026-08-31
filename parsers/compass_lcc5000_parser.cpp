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
    bool parsedAny = false;

    while (true) {
        const qsizetype beginIndex = m_buffer.indexOf(PACKET_BEGIN);
        if (beginIndex < 0) {
            m_buffer.clear();
            return parsedAny;
        }

        m_buffer.remove(0, beginIndex);
        if (m_buffer.size() < 2) {
            return parsedAny;
        }

        const quint8 packetLength =
            static_cast<quint8>(m_buffer.at(1));
        constexpr quint8 minimumPacketLength = 4;
        if (packetLength < minimumPacketLength) {
            m_buffer.remove(0, 1);
            continue;
        }

        const qsizetype frameSize =
            static_cast<qsizetype>(packetLength) + 1;
        if (m_buffer.size() < frameSize) {
            return parsedAny;
        }

        const QByteArray packet = m_buffer.mid(1, packetLength);
        m_buffer.remove(0, frameSize);

        const quint8 calculatedCrc8 =
            BytesSumCrc(packet.first(packetLength - 1));
        const quint8 addr = static_cast<quint8>(packet.at(1));
        const quint8 cmdId = static_cast<quint8>(packet.at(2));
        const quint8 crc8 =
            static_cast<quint8>(packet.at(packetLength - 1));

        if (m_deviceAddr != addr) {
            continue;
        }

        emit lastAnswer(packet);

        if (crc8 != calculatedCrc8) {
            qDebug() << "CompassLCC5000 address:"
                     << addr << "checksum error!";
            continue;
        }

        emit dataReady(packet.mid(3, packetLength - 4), cmdId);
        parsedAny = true;
    }
}
