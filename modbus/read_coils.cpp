#include "read_coils.h"

ReadCoils::ReadCoils(quint16 coilAddress, quint16 coilsCount, AbstractModBusProtocol *protocol, QObject *parent)
    : AbstractCommand(parent),
    protocol(protocol),
    coilAddress(coilAddress),
    coilsCount(coilsCount)
{
    byteCount = static_cast<quint8>((coilsCount + 7) / 8);
}

const QByteArray &ReadCoils::makeCommand()
{
    constexpr quint16 maxCoilsPerRequest = 2000;
    buffer.clear();

    if (coilsCount == 0 || coilsCount > maxCoilsPerRequest ||
        static_cast<quint32>(coilAddress) + coilsCount > 0x10000U) {
        qWarning() << "Invalid Read Coils request:"
                   << "address =" << coilAddress
                   << "count =" << coilsCount;
        cachedCommand.clear();
        return cachedCommand;
    }

    if (cachedPdu.isEmpty()) {
        QDataStream out(&cachedPdu, QIODevice::WriteOnly);
        out.setByteOrder(QDataStream::BigEndian);
        out << cmdID
            << coilAddress
            << coilsCount;
    }

    cachedCommand = protocol->pack(cachedPdu);
    return cachedCommand;
}

QVariant ReadCoils::getValue()
{
    return QVariant::fromValue(coils);
}

bool ReadCoils::tryParse(const QByteArray &data)
{
    buffer.append(data);

    while (true) {
        ModbusFrame frame;
        const auto status = protocol->tryExtractFrame(buffer, frame);

        if (status == ModbusParseStatus::Incomplete) {
            return false;
        }

        if (status == ModbusParseStatus::Invalid || frame.pdu.isEmpty()) {
            continue;
        }

        QDataStream in(frame.pdu);
        in.setByteOrder(QDataStream::BigEndian);

        quint8 function = 0;
        in >> function;

        if (function == (cmdID | 0x80)) {
            quint8 exceptionCode = 0;
            in >> exceptionCode;
            qWarning() << "Modbus exception:" << exceptionCode;
            return true;
        }

        if (function != cmdID) {
            qWarning() << "Unexpected Modbus function:" << function;
            return true;
        }

        quint8 responseByteCount = 0;
        in >> responseByteCount;

        if (in.status() != QDataStream::Ok ||
            responseByteCount != byteCount ||
            frame.pdu.size() != static_cast<qsizetype>(2 + responseByteCount)) {
            qWarning() << "Invalid Read Coils response byte count:"
                       << responseByteCount;
            return true;
        }

        QVector<quint8> parsedCoils;
        parsedCoils.reserve(coilsCount);

        for (quint16 byteIndex = 0; byteIndex < responseByteCount; ++byteIndex) {
            quint8 packedByte = 0;
            in >> packedByte;

            for (quint8 bitIndex = 0;
                 bitIndex < 8 && parsedCoils.size() < coilsCount;
                 ++bitIndex) {
                parsedCoils.append((packedByte >> bitIndex) & 0x01);
            }
        }

        if (in.status() != QDataStream::Ok) {
            return true;
        }

        coils = std::move(parsedCoils);
        return true;
    }
}
