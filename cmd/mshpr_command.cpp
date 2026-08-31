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

    checksum &= 0x7F;
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

        checksum &= 0x7F;
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
    cachedRead.clear();
    cachedWrite.clear();
}

const QByteArray &MShPRCommand::makeCommand()
{
    responseBuffer.clear();

    switch (cmdType) {
    case CommandType::READ:
        return makeReadCommand();
    case CommandType::WRITE:
        return makeWriteCommand();
    default:
        qWarning() << "Unsupported MShPR command type:"
                   << static_cast<int>(cmdType);
        cachedRead.clear();
        return cachedRead;
    }
}

bool MShPRCommand::tryParse(const QByteArray &reply)
{
    responseBuffer.append(reply);

    while (true) {
        const qsizetype beginIndex = responseBuffer.indexOf('>');
        if (beginIndex < 0) {
            responseBuffer.clear();
            return false;
        }

        responseBuffer.remove(0, beginIndex);
        const qsizetype endIndex = responseBuffer.indexOf("\r\n");
        if (endIndex < 0) {
            return false;
        }

        const QByteArray body = responseBuffer.first(endIndex);
        responseBuffer.remove(0, endIndex + 2);

        const qsizetype starIndex = body.lastIndexOf('*');
        if (starIndex < 0 || starIndex + 1 >= body.size()) {
            continue;
        }

        const QByteArray payload = body.first(starIndex + 1);
        quint8 checksum = 0;
        for (const char byte : payload) {
            checksum += static_cast<quint8>(byte);
        }
        checksum &= 0x7F;

        const qsizetype slashIndex = payload.indexOf('/');
        const QString responseAddress = slashIndex > 1
            ? QString::fromUtf8(payload.mid(1, slashIndex - 1))
            : QString();

        if (responseAddress == m_addr &&
            checksum == static_cast<quint8>(body.at(starIndex + 1))) {
            return true;
        }
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
