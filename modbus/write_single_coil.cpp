#include "write_single_coil.h"

WriteSingleCoil::WriteSingleCoil(quint16 coilAddress, AbstractModBusProtocol *protocol, QObject *parent)
    : AbstractCommand(parent),
    protocol(protocol),
    coilAddress(coilAddress)
{}

const QByteArray &WriteSingleCoil::makeCommand()
{
    buffer.clear();
    commandStatus = false;

    if (cachedPdu.isEmpty() || coil != cachedCoil) {
        cachedPdu.clear();
        QDataStream out(&cachedPdu, QIODevice::WriteOnly);
        out.setByteOrder(QDataStream::BigEndian);
        out << cmdID
            << coilAddress
            << static_cast<quint16>(coil ? 0xFF00 : 0x0000);
        cachedCoil = coil;
    }

    cachedCommand = protocol->pack(cachedPdu);
    return cachedCommand;
}

void WriteSingleCoil::setValue(QVariant v)
{
    coil = v.value<bool>();
}

bool WriteSingleCoil::isSuccess()
{
    return commandStatus;
}

bool WriteSingleCoil::tryParse(const QByteArray &data)
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
        quint16 responseValue = 0;
        in >> responseAddress >> responseValue;

        const quint16 expectedValue = cachedCoil ? 0xFF00 : 0x0000;
        commandStatus = in.status() == QDataStream::Ok &&
                        frame.pdu.size() == 5 &&
                        responseAddress == coilAddress &&
                        responseValue == expectedValue;
        return true;
    }
}
