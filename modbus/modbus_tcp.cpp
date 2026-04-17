#include "modbus_tcp.h"

ModBusTcp::ModBusTcp(quint16 slaveID)
    :   slaveID(slaveID)
{}

QByteArray ModBusTcp::pack(QByteArray &pdu)
{
    QByteArray tcpPacket;
    tcpPacket.append(transactionID);

    tcpPacket.append(slaveID);
    tcpPacket.append(pdu);
    return tcpPacket;
}

quint8 ModBusTcp::deviceID()
{
    return slaveID;
}

