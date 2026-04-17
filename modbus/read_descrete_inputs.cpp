#include "read_descrete_inputs.h"

ReadDescreteInputs::ReadDescreteInputs(quint16 inputAddress, quint16 inputsCount, AbstractModBusProtocol *protocol, QObject *parent)
    : AbstractCommand{parent},
    protocol(protocol),
    inputAddress(inputAddress),
    inputsCount(inputsCount)

{
    byteCount = inputsCount * sizeof(quint8);
}

const QByteArray &ReadDescreteInputs::makeCommand()
{
    if (!cachedCommand.isEmpty()) {
        return cachedCommand;
    }
    QDataStream HEADER(&replyHeader, QDataStream::WriteOnly);
    HEADER.setByteOrder(QDataStream::BigEndian);
    HEADER.setFloatingPointPrecision(QDataStream::SinglePrecision);
    HEADER  << protocol->deviceID()
           << cmdID
           << byteCount;

    QDataStream out(&cachedCommand, QDataStream::WriteOnly);
    out.setByteOrder(QDataStream::BigEndian);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out << cmdID
        << inputAddress
        << inputsCount;
    cachedCommand = protocol->pack(cachedCommand);
    return cachedCommand;
}

QVariant ReadDescreteInputs::getValue()
{
    return QVariant::fromValue(inputs);
}

bool ReadDescreteInputs::tryParse(const QByteArray &data)
{
    quint16 packetLength;
    QByteArray packet;
    QDataStream in(&packet, QIODevice::ReadOnly);
    quint16 packetCrc;
    quint16 calculatedCrc;
    bool finded = false;

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
                for (int i = 0; i < inputsCount; i++) {
                    quint8 input;
                    in >> input;
                    inputs.append(input);
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

