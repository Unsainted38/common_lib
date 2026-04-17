#include "mshpr_command.h"

MShPRCommand::MShPRCommand(QString addr, QString cmd, CommandType cmdType)
    :
    m_addr(addr),
    m_cmd(cmd),
    cmdType(cmdType){

}

const QByteArray &MShPRCommand::makeWriteCommand() {
    QByteArray payload;

    if((m_cmd == "AT1" || m_cmd == "ADR") && data.size() == 1) {
        data.push_front("0");
    }

    payload = "<" + m_addr.toUtf8() + "/" + m_cmd.toUtf8() + "=" + data.toUtf8() + "*";

    quint8 checksum = 0;

    for(auto b : payload) {
        checksum += static_cast<quint8>(b);
    }

    checksum &= 0xFF;
    QByteArray csHex;
    csHex.append(checksum);
    QByteArray packet;
    QDataStream o(&packet, QIODevice::WriteOnly);
    o.writeRawData(payload.constData(), payload.size());
    o.writeRawData(csHex.constData(), csHex.size());
    o.writeRawData("\r", 1);
    cachedWrite = packet;
    return cachedWrite;
}

const QByteArray &MShPRCommand::makeReadCommand() {
    if (cachedRead.isEmpty()) {
        QByteArray payload = "<" + m_addr.toUtf8() + "/" + m_cmd.toUtf8() + "*";

        quint8 checksum = 0;

        for(auto b : payload) {
            checksum += static_cast<quint8>(b);
        }

        checksum &= 0xFF;
        QByteArray csHex;
        csHex.append(checksum);
        QByteArray packet;
        QDataStream o(&packet, QIODevice::WriteOnly);
        o.writeRawData(payload.constData(), payload.size());
        o.writeRawData(csHex.constData(), csHex.size());
        o.writeRawData("\r", 1);
        cachedRead = packet;
    }
    return cachedRead;
}

void MShPRCommand::setAddress(QString addr) {
    m_addr = addr;
}

const QByteArray &MShPRCommand::makeCommand()
{
    switch (cmdType) {
    case CommandType::READ:
        return makeReadCommand();
    case CommandType::WRITE:
        return makeWriteCommand();
    }
}

QVariant MShPRCommand::getValue()
{
    return QVariant(data);
}

void MShPRCommand::setValue(QVariant v)
{
    data = v.toString();
}
