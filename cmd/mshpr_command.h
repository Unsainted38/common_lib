#ifndef MSHPRCOMMAND_H
#define MSHPRCOMMAND_H

#include <QObject>
#include <QIODevice>
#include <QDataStream>
#include <QMap>
#include "abstract_command.h"


class MShPRCommand : public AbstractCommand {
    Q_OBJECT
    QString m_addr;
    QString m_cmd;
    QByteArray cachedRead;
    QByteArray cachedWrite;

    const QByteArray &makeWriteCommand();
    const QByteArray &makeReadCommand();
public:
    explicit MShPRCommand(QString addr, QString cmd, ValueType value_type, CommandType cmd_type);

    void setAddress(QString addr);
signals:


    // AbstractCommand interface
public:
    const QByteArray &makeCommand() override;
};

#endif // MSHPRCOMMAND_H
