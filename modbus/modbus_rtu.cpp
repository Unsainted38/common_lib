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

ModbusParseStatus ModBusRtu::tryExtractFrame(QByteArray &buffer, ModbusFrame &frame)
{
    // Удаляем данные до адреса нашего устройства.
    while (!buffer.isEmpty() &&
           static_cast<quint8>(buffer[0]) != slaveID) {
        buffer.remove(0, 1);
    }

    if (buffer.size() < 2) {
        return ModbusParseStatus::Incomplete;
    }

    const quint8 function =
        static_cast<quint8>(buffer[1]);

    qsizetype frameSize = 0;

    if (function & 0x80) {
        // slave + exception function + exception code + CRC
        frameSize = 5;
    } else {
        switch (function) {
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
            if (buffer.size() < 3) {
                return ModbusParseStatus::Incomplete;
            }

            frameSize =
                3 + static_cast<quint8>(buffer[2]) + 2;
            break;

        case 0x05:
        case 0x06:
        case 0x0F:
        case 0x10:
            frameSize = 8;
            break;

        default:
            buffer.remove(0, 1);
            return ModbusParseStatus::Invalid;
        }
    }

    if (buffer.size() < frameSize) {
        return ModbusParseStatus::Incomplete;
    }

    const QByteArray adu = buffer.left(frameSize);

    const quint16 receivedCrc =
        static_cast<quint8>(adu[frameSize - 2]) |
        (static_cast<quint16>(
             static_cast<quint8>(adu[frameSize - 1])) << 8);

    if (receivedCrc != GetCrc16(adu)) {
        // frameSize мог быть получен из повреждённого byte count.
        // Сдвигаемся на один байт, чтобы не удалить следующий корректный кадр.
        buffer.remove(0, 1);
        return ModbusParseStatus::Invalid;
    }

    buffer.remove(0, frameSize);

    frame.transactionId = 0;
    frame.deviceId = static_cast<quint8>(adu[0]);

    // Удаляем device ID и CRC.
    frame.pdu = adu.mid(1, frameSize - 3);

    return ModbusParseStatus::Complete;
}
