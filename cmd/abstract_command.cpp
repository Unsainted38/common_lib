#include "abstract_command.h"

AbstractCommand::AbstractCommand(ValueType value_type, CommandType cmd_type, QObject *parent)
    :   QObject(parent),
        m_type(value_type),
        cmdType(cmd_type) {

}

void AbstractCommand::processData(const QByteArray &data, quint16 regAddr) {
    qDebug() << "[WARNING!!!] In AbstractCommand class processData() [WARNING!!!]";
}
