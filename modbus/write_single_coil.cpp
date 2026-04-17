#include "write_single_coil.h"

WriteSingleCoil::WriteSingleCoil(quint16 coilAddress, AbstractModBusProtocol *protocol, QObject *parent)
    : AbstractCommand(parent),
    protocol(protocol),
    coilAddress(coilAddress)
{
    byteCount = sizeof(quint8);
}

const QByteArray &WriteSingleCoil::makeCommand()
{
    if (!cachedCommand.isEmpty() && (coil == cachedCoil)) {
        return cachedCommand;
    }
    QDataStream HEADER(&replyHeader, QDataStream::WriteOnly);
    HEADER.setByteOrder(QDataStream::BigEndian);
    HEADER.setFloatingPointPrecision(QDataStream::SinglePrecision);
    HEADER  << protocol->deviceID()
           << cmdID
           << byteCount;

    cachedCoil = coil;
    QDataStream out(&cachedCommand, QDataStream::WriteOnly);
    out.setByteOrder(QDataStream::BigEndian);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out << cmdID
        << coilAddress
        << (cachedCoil ? 0xFF00 : 0x0000);
    cachedCommand = protocol->pack(cachedCommand);
    return cachedCommand;
}

void WriteSingleCoil::setValue(QVariant v)
{
    coil = v.value<bool>();
}

bool WriteSingleCoil::isSuccess()
{
    return commandStatus;
}

bool WriteSingleCoil::tryParse(const QByteArray &data)
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
                in.device()->seek(0);
                in.device()->seek(replyHeader.size());
                in.setByteOrder(QDataStream::BigEndian);
                    in >> coil;
                if (coil == cachedCoil) {
                    commandStatus = true;
                }
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
