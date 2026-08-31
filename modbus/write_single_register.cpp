#include "write_single_register.h"

WriteSingleRegister::WriteSingleRegister(quint16 regAddress, AbstractModBusProtocol *protocol, QObject *parent)
    : AbstractCommand(parent),
    protocol(protocol),
    registerAddress(regAddress)
{}

const QByteArray &WriteSingleRegister::makeCommand()
{
    buffer.clear();
    commandStatus = false;

    if (cachedPdu.isEmpty() || reg != cachedReg) {
        cachedPdu.clear();
        QDataStream out(&cachedPdu, QIODevice::WriteOnly);
        out.setByteOrder(QDataStream::BigEndian);
        out << cmdID
            << registerAddress
            << reg;
        cachedReg = reg;
    }

    cachedCommand = protocol->pack(cachedPdu);
    return cachedCommand;
}

void WriteSingleRegister::setValue(QVariant v)
{
    reg = v.value<quint16>();
}

bool WriteSingleRegister::isSuccess()
{
    return  commandStatus;
}

bool WriteSingleRegister::tryParse(const QByteArray &data)
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

        commandStatus = in.status() == QDataStream::Ok &&
                        frame.pdu.size() == 5 &&
                        responseAddress == registerAddress &&
                        responseValue == cachedReg;
        return true;
    }
}
