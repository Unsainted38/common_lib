#include "ubpch_parser.h"
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
    emit lastAnswer(packet);
    QDataStream in(&packet, QIODevice::ReadOnly);
    in.setByteOrder(QDataStream::LittleEndian);
    in >> start
       >> DST
       >> SRC
       >> cmdId
       >> regAdr;
    QByteArray data = packet.mid(in.device()->pos(), packet.length() - in.device()->pos() - 4);
    packet.remove(0, packet.length() - 4);
    in.device()->seek(0);
    in >> crc
       >> stop;

    if(cmdId == READ_REPLY_CMD || cmdId == WRITE_REPLY_CMD) {
        emit dataReady(data, regAdr);
        emit statusOnline();
    }

    m_buffer.remove(0, endIndex + 2); // удаляем весь полученный пакет и все, что было до него
    return true;
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


