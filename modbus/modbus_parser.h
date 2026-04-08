#ifndef MODBUS_PARSER_H
#define MODBUS_PARSER_H

#include <QObject>

class ModBusParser : public QObject
{
    Q_OBJECT
    QByteArray buffer;
public:
    explicit ModBusParser(QObject *parent = nullptr);
    bool parseReply(const QByteArray &data);
signals:
};

#endif // MODBUS_PARSER_H
