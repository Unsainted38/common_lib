#include "modbus_rtu.h"

ModBusRtu::ModBusRtu(quint8 slaveID)
    : slaveID(slaveID)
{}

QByteArray ModBusRtu::pack(const QByteArray &pdu)
{
    QByteArray rtuPacket;
    rtuPacket.append(slaveID);
    rtuPacket.append(pdu);
    quint16 crc16 = GetCrc16_n(rtuPacket);
    rtuPacket.append(BitUtils::Low(crc16));
    rtuPacket.append(BitUtils::High(crc16));
    return rtuPacket;
}

quint8 ModBusRtu::deviceID()
{
    return slaveID;
}
