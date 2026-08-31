#include "ubpch_parser.h"
#include "crc16.h"

const QByteArray HEADER = QByteArray::fromHex("FE FE");
const QByteArray END = QByteArray::fromHex("FC FC");
const quint8 ERROR_CMD = 0x0A;
const quint8 WRITE_REPLY_CMD = 0x06;
const quint8 READ_REPLY_CMD = 0x04;


UBPChParser::UBPChParser(QObject *parent)
    : QObject{parent} {

}

UBPChParser::UBPChParser(quint8 deviceAddr, quint8 masterAddr,
                         QObject *parent)
    : QObject(parent),
      m_deviceAddr(deviceAddr),
      m_masterAddr(masterAddr),
      m_filterAddresses(true)
{
}

bool UBPChParser::parseReply(const QByteArray &reply) {
    constexpr qsizetype frameMarkerSize = 2;
    constexpr qsizetype trailerSize = 4;     // CRC16 + FC FC
    constexpr qsizetype minimumPacketSize = 11;

    m_buffer.append(reply);
    bool parsedAny = false;

    while (true) {
        const qsizetype headerIndex = m_buffer.indexOf(HEADER);
        if (headerIndex < 0) {
            // Сохраняем одиночный FE: он может быть первой половиной заголовка.
            if (!m_buffer.isEmpty() &&
                static_cast<quint8>(m_buffer.back()) == 0xFE) {
                m_buffer = m_buffer.last(1);
            } else {
                m_buffer.clear();
            }
            return parsedAny;
        }

        if (headerIndex > 0) {
            m_buffer.remove(0, headerIndex);
        }

        const qsizetype endIndex = m_buffer.indexOf(END, HEADER.size());
        const qsizetype nextHeaderIndex =
            m_buffer.indexOf(HEADER, HEADER.size());

        if (endIndex < 0) {
            if (nextHeaderIndex > 0) {
                m_buffer.remove(0, nextHeaderIndex);
                continue;
            }
            return parsedAny;
        }

        if (nextHeaderIndex > 0 && nextHeaderIndex < endIndex) {
            m_buffer.remove(0, nextHeaderIndex);
            continue;
        }

        const QByteArray rawFrame = m_buffer.first(endIndex + END.size());
        m_buffer.remove(0, endIndex + END.size());
        const QByteArray packet = unpack(rawFrame);

        if (packet.size() < minimumPacketSize) {
            continue;
        }

        emit lastAnswer(packet);

        quint16 start = 0;
        quint8 dst = 0;
        quint8 src = 0;
        quint8 cmdId = 0;
        quint16 regAdr = 0;
        quint16 crc = 0;
        quint16 stop = 0;

        QDataStream in(packet);
        in.setByteOrder(QDataStream::LittleEndian);
        in >> start >> dst >> src >> cmdId >> regAdr;

        const qsizetype dataOffset = in.device()->pos();
        const QByteArray data = packet.mid(
            dataOffset, packet.size() - dataOffset - trailerSize);
        const QByteArray crcPayload = packet.mid(
            frameMarkerSize,
            packet.size() - frameMarkerSize - trailerSize);

        in.device()->seek(packet.size() - trailerSize);
        in >> crc >> stop;

        const bool valid =
            in.status() == QDataStream::Ok &&
            start == 0xFEFE &&
            stop == 0xFCFC &&
            (!m_filterAddresses ||
             (dst == m_masterAddr && src == m_deviceAddr)) &&
            GetCrc16_ubpch(crcPayload) == crc;
        const bool supportedCommand =
            cmdId == READ_REPLY_CMD ||
            cmdId == WRITE_REPLY_CMD ||
            cmdId == ERROR_CMD;

        if (!valid || !supportedCommand) {
            continue;
        }

        if (cmdId != ERROR_CMD) {
            emit dataReady(data, regAdr);
        }
        emit statusOnline();
        parsedAny = true;
    }
}

QByteArray UBPChParser::unpack(const QByteArray &frame) {
    QByteArray result;
    result.reserve(frame.size());

    for(int i = 0; i < frame.size(); ++i) {
        quint8 byte = static_cast<quint8>(frame[i]);


        if((byte == 0xFE || byte == 0xFC) && i + 1 < frame.size()) {
            if(frame[i + 1] == 0x00) {
                result.append(static_cast<char>(byte));
                ++i;
                continue;
            }
        }


        result.append(static_cast<char>(byte));
    }

    return result;

}
