#include "write_multiple_registers.h"

WriteMultipleRegisters::WriteMultipleRegisters(quint16 regAddress, quint16 regsCount, AbstractModBusProtocol *protocol, QObject *parent)
    : AbstractCommand(parent),
    protocol(protocol),
    registerAddress(regAddress),
    registersCount(regsCount)
{
    byteCount = regsCount * sizeof(quint16);
}

const QByteArray &WriteMultipleRegisters::makeCommand()
{
    if (!cachedCommand.isEmpty() && (regs == cachedRegs)) {
        return cachedCommand;
    }
    QDataStream HEADER(&replyHeader, QDataStream::WriteOnly);
    HEADER.setByteOrder(QDataStream::BigEndian);
    HEADER.setFloatingPointPrecision(QDataStream::SinglePrecision);
    HEADER  << protocol->deviceID()
           << cmdID
           << byteCount;

    cachedRegs = regs;
    QDataStream out(&cachedCommand, QDataStream::WriteOnly);
    out.setByteOrder(QDataStream::BigEndian);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out << cmdID
        << registerAddress
        << registersCount
        << static_cast<quint16>(cachedRegs.size());
    for (const quint16 &word : cachedRegs) {
        out << word;
    }
    cachedCommand = protocol->pack(cachedCommand);
    return cachedCommand;
}

void WriteMultipleRegisters::setValue(QVariant v)
{
    regs = v.value<QVector<quint16>>();
}

bool WriteMultipleRegisters::isSuccess()
{
    return commandStatus;
}

bool WriteMultipleRegisters::tryParse(const QByteArray &data)
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
