#ifndef ABSTRACTCOMMAND_H
#define ABSTRACTCOMMAND_H

#include <QObject>
#include <QIODevice>
#include <QDataStream>
#include <QDebug>
#include <QList>
#include <QVariant>

enum ValueType {
    QINT8 = 7,
    QUINT8 = 8,
    QUINT16 = 16,
    QUINT32 = 32,
    QINT32 = 31,
    QSTRING = 25,
    FLOAT = 26,
    DOUBLE = 27,
    QBYTEARRAY = 28,
    QLISTQUINT16 = 29,
};

enum class CommandType {
    READ,
    WRITE,
    MODBUS,
    READWRITE,
};

class AbstractCommand : public QObject {
    Q_OBJECT
public:
    explicit AbstractCommand(QObject *parent = nullptr);
    virtual const QByteArray &makeCommand() = 0;

    virtual QVariant getValue();
    virtual void setValue(QVariant v);
    virtual bool isSuccess();

    virtual bool tryParse(const QByteArray &data);

public slots:
    virtual void processData(const QByteArray &data, quint16 regAddr);
    virtual void processData(const QByteArray &data);
signals:
protected:
};

#endif // ABSTRACTCOMMAND_H
