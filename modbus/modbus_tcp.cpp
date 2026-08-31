#include "modbus_tcp.h"
#include <QDataStream>

ModBusTcp::ModBusTcp(quint16 slaveID)
    :   slaveID(slaveID)
{}

QByteArray ModBusTcp::pack(const QByteArray &pdu)
{
    if (pdu.size() > 253) {
        qWarning() << "Modbus TCP PDU is too large:" << pdu.size();
        return {};
    }

    QByteArray tcpPacket;
    QDataStream out(&tcpPacket, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::BigEndian);

    const quint16 length = static_cast<quint16>(pdu.size() + 1);

    out << transactionID++
        << quint16{0}
        << length
        << slaveID;
    tcpPacket.append(pdu);
    return tcpPacket;
}

quint8 ModBusTcp::deviceID()
{
    return slaveID;
}

