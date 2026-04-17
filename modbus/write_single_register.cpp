#include "write_single_register.h"

WriteSingleRegister::WriteSingleRegister(quint16 regAddress, AbstractModBusProtocol *protocol, QObject *parent)
    : AbstractCommand(parent),
    protocol(protocol),
    registerAddress(regAddress)
{
    byteCount = sizeof(quint16);
}

const QByteArray &WriteSingleRegister::makeCommand()
{
    if (!cachedCommand.isEmpty() && (reg == cachedReg)) {
        return cachedCommand;
    }

    QDataStream HEADER(&replyHeader, QDataStream::WriteOnly);
    HEADER.setByteOrder(QDataStream::BigEndian);
    HEADER.setFloatingPointPrecision(QDataStream::SinglePrecision);
    HEADER  << protocol->deviceID()
           << cmdID
           << byteCount;

    cachedReg = reg;
    QDataStream out(&cachedCommand, QDataStream::WriteOnly);
    out.setByteOrder(QDataStream::BigEndian);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out << cmdID
        << registerAddress
        << cachedReg;
    cachedCommand = protocol->pack(cachedCommand);
    return cachedCommand;
}

void WriteSingleRegister::setValue(QVariant v)
{
    reg = v.value<quint16>();
}

bool WriteSingleRegister::isSuccess()
{
    return  commandStatus;
}

bool WriteSingleRegister::tryParse(const QByteArray &data)
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
                in >> reg;
                if (reg == cachedReg) {
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
