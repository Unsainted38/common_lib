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
    QString data;
    QByteArray cachedRead;
    QByteArray cachedWrite;
    QByteArray responseBuffer;
    CommandType cmdType;
    ValueType valueType;

    const QByteArray &makeWriteCommand();
    const QByteArray &makeReadCommand();
public:
    explicit MShPRCommand(QString addr, QString cmd, CommandType cmdType);


    void setAddress(QString addr);
signals:


    // AbstractCommand interface
public:
    const QByteArray &makeCommand() override;
    bool tryParse(const QByteArray &data) override;

    // AbstractCommand interface
public:
    QVariant getValue() override;
    void setValue(QVariant v) override;
};

#endif // MSHPRCOMMAND_H
