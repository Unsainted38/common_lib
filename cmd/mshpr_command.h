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
public:
    explicit MShPRCommand(QString addr, QString cmd, ValueType type);
    const QByteArray &makeWriteCommand() override;
    const QByteArray &makeReadCommand() override;
    void setAddress(QString addr);
signals:

};

#endif // MSHPRCOMMAND_H
