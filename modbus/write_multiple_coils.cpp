#include "write_multiple_coils.h"

WriteMultipleCoils::WriteMultipleCoils(quint16 coilAddress, quint16 coilsCount, AbstractModBusProtocol *protocol, QObject *parent)
    : AbstractCommand(parent),
    protocol(protocol),
    coilAddress(coilAddress),
    coilsCount(coilsCount)
{}

const QByteArray &WriteMultipleCoils::makeCommand()
{
    constexpr quint16 maxCoilsPerRequest = 1968;
    buffer.clear();
    commandStatus = false;

    if (coilsCount == 0 ||
        coilsCount > maxCoilsPerRequest ||
        static_cast<quint32>(coilAddress) + coilsCount > 0x10000U ||
        coils.size() != static_cast<qsizetype>(coilsCount)) {
        qWarning() << "Invalid Write Multiple Coils request:"
                   << "coilsCount =" << coilsCount
                   << "values count =" << coils.size();
        cachedCommand.clear();
        return cachedCommand;
    }

    if (cachedPdu.isEmpty() || coils != cachedCoils) {
        const quint8 payloadByteCount =
            static_cast<quint8>((coilsCount + 7) / 8);

        QByteArray packedCoils(payloadByteCount, '\0');
        for (quint16 i = 0; i < coilsCount; ++i) {
            if (coils.at(i) != 0) {
                packedCoils[i / 8] = static_cast<char>(
                    static_cast<quint8>(packedCoils.at(i / 8)) |
                    static_cast<quint8>(1U << (i % 8)));
            }
        }

        cachedPdu.clear();
        QDataStream out(&cachedPdu, QIODevice::WriteOnly);
        out.setByteOrder(QDataStream::BigEndian);
        out << cmdID
            << coilAddress
            << coilsCount
            << payloadByteCount;
        out.writeRawData(packedCoils.constData(),
                         static_cast<int>(packedCoils.size()));
        cachedCoils = coils;
    }

    cachedCommand = protocol->pack(cachedPdu);
    return cachedCommand;
}

void WriteMultipleCoils::setValue(QVariant v)
{
    coils = v.value<QVector<quint8>>();
}

bool WriteMultipleCoils::isSuccess()
{
    return commandStatus;
}

bool WriteMultipleCoils::tryParse(const QByteArray &data)
{
    commandStatus = false;
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

        quint16 responseAddress = 0;
        quint16 responseCount = 0;
        in >> responseAddress >> responseCount;

        commandStatus = in.status() == QDataStream::Ok &&
                        frame.pdu.size() == 5 &&
                        responseAddress == coilAddress &&
                        responseCount == coilsCount;
        return true;
    }
}
