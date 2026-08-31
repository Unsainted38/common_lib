#include "write_multiple_registers.h"

WriteMultipleRegisters::WriteMultipleRegisters(quint16 regAddress, quint16 regsCount, AbstractModBusProtocol *protocol, QObject *parent)
    : AbstractCommand(parent),
    protocol(protocol),
    registerAddress(regAddress),
    registersCount(regsCount)
{
}

const QByteArray& WriteMultipleRegisters::makeCommand()
{
    constexpr quint16 maxRegistersPerRequest = 123;
    buffer.clear();
    commandStatus = false;

    if (registersCount == 0 ||
        registersCount > maxRegistersPerRequest ||
        static_cast<quint32>(registerAddress) + registersCount > 0x10000U ||
        regs.size() != static_cast<qsizetype>(registersCount)) {
        qWarning() << "Invalid Write Multiple Registers request:"
                   << "registersCount =" << registersCount
                   << "values count =" << regs.size();

        cachedCommand.clear();
        return cachedCommand;
    }

    if (cachedPdu.isEmpty() || regs != cachedRegs) {
        const quint8 payloadByteCount =
            static_cast<quint8>(registersCount * sizeof(quint16));

        cachedPdu.clear();
        QDataStream out(&cachedPdu, QIODevice::WriteOnly);
        out.setByteOrder(QDataStream::BigEndian);

        out << cmdID
            << registerAddress
            << registersCount
            << payloadByteCount;

        for (const quint16 value : std::as_const(regs)) {
            out << value;
        }

        cachedRegs = regs;
    }

    cachedCommand = protocol->pack(cachedPdu);
    return cachedCommand;
}

void WriteMultipleRegisters::setValue(QVariant v)
{
    regs = v.value<QVector<quint16>>();
}

bool WriteMultipleRegisters::isSuccess()
{
    return commandStatus;
}

bool WriteMultipleRegisters::tryParse(const QByteArray &data)
{
    buffer.append(data);
    commandStatus = false;

    while (true) {
        ModbusFrame frame;

        const ModbusParseStatus status =
            protocol->tryExtractFrame(buffer, frame);

        if (status == ModbusParseStatus::Incomplete) {
            return false;
        }

        if (status == ModbusParseStatus::Invalid) {
            continue;
        }

        if (frame.pdu.isEmpty()) {
            continue;
        }

        QDataStream in(frame.pdu);
        in.setByteOrder(QDataStream::BigEndian);

        quint8 function = 0;
        in >> function;

        // Exception-ответ тоже завершает ожидание.
        if (function == (cmdID | 0x80)) {
            quint8 exceptionCode = 0;
            in >> exceptionCode;

            qWarning() << "Modbus exception:"
                       << exceptionCode;

            commandStatus = false;
            return true;
        }

        if (function != cmdID) {
            qWarning() << "Unexpected Modbus function:" << function;
            return true;
        }

        quint16 responseAddress = 0;
        quint16 responseCount = 0;

        in >> responseAddress
            >> responseCount;

        commandStatus = in.status() == QDataStream::Ok &&
                        frame.pdu.size() == 5 &&
                        responseAddress == registerAddress &&
                        responseCount == registersCount;

        // Ответ получен, requester может снять блокировку.
        return true;
    }
}
