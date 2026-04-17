#include "abstract_command.h"

AbstractCommand::AbstractCommand(QObject *parent)
    :   QObject(parent) {

}

QVariant AbstractCommand::getValue()
{
    qDebug() << "[WARNING!!!] In AbstractCommand class getValue() [WARNING!!!]";
}

void AbstractCommand::setValue(QVariant v)
{
    Q_UNUSED(v);
    qDebug() << "[WARNING!!!] In AbstractCommand class setValue() [WARNING!!!]";
}

bool AbstractCommand::isSuccess()
{
    qDebug() << "[WARNING!!!] In AbstractCommand class isSuccess() [WARNING!!!]";
}

void AbstractCommand::processData(const QByteArray &data, quint16 regAddr) {
    Q_UNUSED(data);
    Q_UNUSED(regAddr);
    qDebug() << "[WARNING!!!] In AbstractCommand class processData() [WARNING!!!]";
}

void AbstractCommand::processData(const QByteArray &data)
{
    Q_UNUSED(data);
    qDebug() << "[WARNING!!!] In AbstractCommand class processData() [WARNING!!!]";
}

bool AbstractCommand::tryParse(const QByteArray &data)
{
    Q_UNUSED(data);
    qDebug() << "[WARNING!!!] In AbstractCommand class tryParse() [WARNING!!!]";
}
