#include "read_holding_registers.h"

ReadHoldingRegisters::ReadHoldingRegisters(quint16 regAddress, quint16 regsCount, AbstractModBusProtocol *protocol, QObject *parent)
    : AbstractCommand(parent),
    protocol(protocol),
    registerAddress(regAddress),
    registersCount(regsCount)
{
    byteCount = static_cast<quint8>(registersCount * sizeof(quint16));
}

const QByteArray &ReadHoldingRegisters::makeCommand()
{
    constexpr quint16 maxRegistersPerRequest = 125;
    buffer.clear();

    if (registersCount == 0 || registersCount > maxRegistersPerRequest ||
        static_cast<quint32>(registerAddress) + registersCount > 0x10000U) {
        qWarning() << "Invalid Read Holding Registers request:"
                   << "address =" << registerAddress
                   << "count =" << registersCount;
        cachedCommand.clear();
        return cachedCommand;
    }

    if (cachedPdu.isEmpty()) {
        QDataStream out(&cachedPdu, QIODevice::WriteOnly);
        out.setByteOrder(QDataStream::BigEndian);
        out << cmdID
            << registerAddress
            << registersCount;
    }

    cachedCommand = protocol->pack(cachedPdu);
    return cachedCommand;
}

QVariant ReadHoldingRegisters::getValue()
{
    return QVariant::fromValue(regs);
}

bool ReadHoldingRegisters::tryParse(const QByteArray &data)
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
            qWarning() << "Invalid Read Holding Registers response byte count:"
                       << responseByteCount;
            return true;
        }

        QVector<quint16> parsedRegs;
        parsedRegs.reserve(registersCount);
        for (quint16 i = 0; i < registersCount; ++i) {
            quint16 value = 0;
            in >> value;
            parsedRegs.append(value);
        }

        if (in.status() != QDataStream::Ok) {
            return true;
        }

        regs = std::move(parsedRegs);
        return true;
    }
}
