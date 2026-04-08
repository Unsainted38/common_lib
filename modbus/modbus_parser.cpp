#include "modbus_parser.h"

ModBusParser::ModBusParser(QObject *parent)
    : QObject(parent)
{}

bool ModBusParser::parseReply(const QByteArray &data)
{
    buffer.append(data);
    if (buffer.isEmpty()) {
        return false;
    }



    return true;
}
