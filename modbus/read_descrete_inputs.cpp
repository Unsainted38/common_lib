#include "read_descrete_inputs.h"

ReadDescreteInputs::ReadDescreteInputs(quint16 inputAddress, quint16 inputsCount, AbstractModBusProtocol *protocol, QObject *parent)
    : AbstractCommand{parent},
    protocol(protocol),
    inputAddress(inputAddress),
    inputsCount(inputsCount)

{
    byteCount = static_cast<quint8>((inputsCount + 7) / 8);
}

const QByteArray &ReadDescreteInputs::makeCommand()
{
    constexpr quint16 maxInputsPerRequest = 2000;
    buffer.clear();

    if (inputsCount == 0 || inputsCount > maxInputsPerRequest ||
        static_cast<quint32>(inputAddress) + inputsCount > 0x10000U) {
        qWarning() << "Invalid Read Discrete Inputs request:"
                   << "address =" << inputAddress
                   << "count =" << inputsCount;
        cachedCommand.clear();
        return cachedCommand;
    }

    if (cachedPdu.isEmpty()) {
        QDataStream out(&cachedPdu, QIODevice::WriteOnly);
        out.setByteOrder(QDataStream::BigEndian);
        out << cmdID
            << inputAddress
            << inputsCount;
    }

    cachedCommand = protocol->pack(cachedPdu);
    return cachedCommand;
}

QVariant ReadDescreteInputs::getValue()
{
    return QVariant::fromValue(inputs);
}

bool ReadDescreteInputs::tryParse(const QByteArray &data)
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
            qWarning() << "Invalid Read Discrete Inputs response byte count:"
                       << responseByteCount;
            return true;
        }

        QVector<quint8> parsedInputs;
        parsedInputs.reserve(inputsCount);

        for (quint16 byteIndex = 0; byteIndex < responseByteCount; ++byteIndex) {
            quint8 packedByte = 0;
            in >> packedByte;

            for (quint8 bitIndex = 0;
                 bitIndex < 8 && parsedInputs.size() < inputsCount;
                 ++bitIndex) {
                parsedInputs.append((packedByte >> bitIndex) & 0x01);
            }
        }

        if (in.status() != QDataStream::Ok) {
            return true;
        }

        inputs = std::move(parsedInputs);
        return true;
    }
}
