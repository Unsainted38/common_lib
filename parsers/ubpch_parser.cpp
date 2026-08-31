#include "ubpch_parser.h"
#include "crc16.h"

const QByteArray HEADER = QByteArray::fromHex("FE FE");
const QByteArray END = QByteArray::fromHex("FC FC");
const quint8 ERROR_CMD = 0x0A;
const quint8 WRITE_REPLY_CMD = 0x06;
const quint8 READ_REPLY_CMD = 0x04;
const quint8 WRITE_CMD = 0x05;
const quint8 READ_CMD = 0x03;


UBPChParser::UBPChParser(QObject *parent)
    : QObject{parent} {

}

bool UBPChParser::parseReply(const QByteArray &reply) {
    constexpr qsizetype frameMarkerSize = 2;
    constexpr qsizetype trailerSize = 4;     // CRC16 + FC FC
    constexpr qsizetype minimumPacketSize = 11;

    m_buffer.append(reply);

    int headerIndex = m_buffer.indexOf(HEADER);

    while(headerIndex < 0) {
        return false;
    }

    m_buffer.remove(0, headerIndex);
    headerIndex = 0;
    int endIndex = m_buffer.indexOf(END);

    while(endIndex < 0 || endIndex < headerIndex) {
        return false;
    }

    uint16_t start;
    uint8_t DST; // Отправитель
    uint8_t SRC; // Получатель
    uint8_t cmdId; // код команды
    uint16_t regAdr; // адрес регистра
    uint16_t crc; // контрольная сумма
    uint16_t stop;
    QByteArray packet = unpack(m_buffer.mid(headerIndex, endIndex + 2));

    if (packet.size() < minimumPacketSize) {
        m_buffer.remove(0, endIndex + 2);
        return false;
    }

    emit lastAnswer(packet);
    QDataStream in(&packet, QIODevice::ReadOnly);
    in.setByteOrder(QDataStream::LittleEndian);
    in >> start
       >> DST
       >> SRC
       >> cmdId
       >> regAdr;

    const qsizetype dataOffset = in.device()->pos();
    const QByteArray data =
        packet.mid(dataOffset, packet.size() - dataOffset - trailerSize);

    const QByteArray crcPayload =
        packet.mid(frameMarkerSize,
                   packet.size() - frameMarkerSize - trailerSize);

    in.device()->seek(packet.size() - trailerSize);

    in >> crc
       >> stop;
    const quint16 calculatedCrc = GetCrc16_ubpch(crcPayload);
    const bool valid =
        in.status() == QDataStream::Ok &&
        start == 0xFEFE &&
        stop == 0xFCFC &&
        calculatedCrc == crc;
    const bool supportedCommand =
        cmdId == READ_REPLY_CMD || cmdId == WRITE_REPLY_CMD;

    if (valid && supportedCommand) {
        emit dataReady(data, regAdr);
        emit statusOnline();
    }

    m_buffer.remove(0, endIndex + 2);
    return valid && supportedCommand;
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


