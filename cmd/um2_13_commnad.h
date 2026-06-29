#ifndef UM2_13_COMMNAD_H
#define UM2_13_COMMNAD_H

#include <QObject>
#include "abstract_command.h"

class UM2_13Commnad : public AbstractCommand
{
    Q_OBJECT
    quint16 cmdId;
    quint16 length;
    CommandType cmdType;
    ValueType valueType;
    QVariant value;
    QVariant cachedValue;
    QByteArray cachedCommand;
    QByteArray buffer;
public:
    explicit UM2_13Commnad(quint16 cmdId, quint16 length, ValueType valueType, CommandType cmdType);

    // AbstractCommand interface
public:
    const QByteArray &makeCommand() override;
    QVariant getValue() override;
    void setValue(QVariant v) override;
    bool tryParse(const QByteArray &data) override;
signals:
    void translateLastAnswer(QByteArray);
};

#endif // UM2_13_COMMNAD_H
