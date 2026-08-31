#include "modbus_tcp.h"
#include <QDataStream>
#include <QDebug>
#include <QIODevice>

ModBusTcp::ModBusTcp(quint16 slaveID)
    :   slaveID(slaveID)
{}

QByteArray ModBusTcp::pack(const QByteArray &pdu)
{
    hasExpectedTransactionID = false;

    if (pdu.size() > 253) {
        qWarning() << "Modbus TCP PDU is too large:" << pdu.size();
        return {};
    }

    QByteArray tcpPacket;
    QDataStream out(&tcpPacket, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::BigEndian);

    const quint16 length = static_cast<quint16>(pdu.size() + 1);

    expectedTransactionID = transactionID++;
    hasExpectedTransactionID = true;

    out << expectedTransactionID
        << protocolID
        << length
        << slaveID;
    tcpPacket.append(pdu);
    return tcpPacket;
}

quint8 ModBusTcp::deviceID()
{
    return slaveID;
}

ModbusParseStatus ModBusTcp::tryExtractFrame(QByteArray &buffer, ModbusFrame &frame)
{
    constexpr qsizetype mbapSize = 7;

    if (buffer.size() < mbapSize) {
        return ModbusParseStatus::Incomplete;
    }

    QDataStream header(buffer);
    header.setByteOrder(QDataStream::BigEndian);

    quint16 transactionId = 0;
    quint16 protocolId = 0;
    quint16 length = 0;
    quint8 unitId = 0;

    header >> transactionId
        >> protocolId
        >> length
        >> unitId;

    // Length включает Unit ID и PDU.
    if (header.status() != QDataStream::Ok ||
        protocolId != protocolID ||
        length < 2 ||
        length > 254) {
        buffer.remove(0, 1);
        return ModbusParseStatus::Invalid;
    }

    const qsizetype frameSize = 6 + length;

    if (buffer.size() < frameSize) {
        return ModbusParseStatus::Incomplete;
    }

    const QByteArray adu = buffer.left(frameSize);
    buffer.remove(0, frameSize);

    const bool isExpectedTransaction =
        hasExpectedTransactionID &&
        transactionId == expectedTransactionID;

    if (!isExpectedTransaction || unitId != slaveID) {
        return ModbusParseStatus::Invalid;
    }

    hasExpectedTransactionID = false;

    frame.transactionId = transactionId;
    frame.deviceId = unitId;
    frame.pdu = adu.mid(mbapSize, length - 1);

    return ModbusParseStatus::Complete;
}
