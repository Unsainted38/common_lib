#include "write_multiple_coils.h"

WriteMultipleCoils::WriteMultipleCoils(quint16 coilAddress, quint16 coilsCount, AbstractModBusProtocol *protocol, QObject *parent)
    : AbstractCommand(parent),
    protocol(protocol),
    coilAddress(coilAddress),
    coilsCount(coilsCount)
{
    byteCount = coilsCount * sizeof(quint8);
}

const QByteArray &WriteMultipleCoils::makeCommand()
{
    if (!cachedCommand.isEmpty() && (coils == cachedCoils)) {
        return cachedCommand;
    }
    QDataStream HEADER(&replyHeader, QDataStream::WriteOnly);
    HEADER.setByteOrder(QDataStream::BigEndian);
    HEADER.setFloatingPointPrecision(QDataStream::SinglePrecision);
    HEADER  << protocol->deviceID()
           << cmdID
           << byteCount;

    cachedCoils = coils;
    QDataStream out(&cachedCommand, QDataStream::WriteOnly);
    out.setByteOrder(QDataStream::BigEndian);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out << cmdID
        << coilAddress
        << coilsCount
        << static_cast<quint8>((coilsCount + 7) / 8);
    for (const quint8 &word : cachedCoils) {
        out << word;
    }
    cachedCommand = protocol->pack(cachedCommand);
    return cachedCommand;
}

void WriteMultipleCoils::setValue(QVariant v)
{
    coils = v.value<QVector<quint8>>();
}

bool WriteMultipleCoils::isSuccess()
{
    return commandStatus;
}

bool WriteMultipleCoils::tryParse(const QByteArray &data)
{
    quint16 packetLength;
    QByteArray packet;
    QDataStream in(&packet, QIODevice::ReadOnly);
    quint16 packetCrc;
    quint16 calculatedCrc;
    bool finded = false;
    commandStatus = false;

    if (replyHeader.size() == 0) {
        return false;
    }

    buffer.append(data);
    int replyHeaderIndex = buffer.indexOf(replyHeader);
    if (replyHeaderIndex < 0 && (buffer.size() > replyHeader.size())) {
        buffer.remove(0, buffer.size() - replyHeader.size());
        return finded;
    }

    while (replyHeaderIndex >=0) {
        packetLength = replyHeader.size() + byteCount + 2;
        if (buffer.size() >= replyHeaderIndex + packetLength) {
            packet = buffer.mid(replyHeaderIndex, packetLength);
            in.setByteOrder(QDataStream::LittleEndian);
            in.device()->seek(packet.size() - 2);
            in >> packetCrc;
            calculatedCrc = GetCrc16(packet);
            if (packetCrc == calculatedCrc) {
                commandStatus = true;
                finded = true;
            }

        } else {
            break;
        }
        buffer.remove(0, replyHeaderIndex + packetLength);
        replyHeaderIndex = buffer.indexOf(replyHeader);
    }
    return finded;
}
