#include "abstract_command.h"

AbstractCommand::AbstractCommand(ValueType type, QObject *parent)
    : QObject(parent),
      m_type(type) {

}

void AbstractCommand::processData(const QByteArray &data, quint16 regAddr) {
    qDebug() << "[WARNING!!!] In AbstractCommand class processData() [WARNING!!!]";
}
