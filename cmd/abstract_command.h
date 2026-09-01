#ifndef ABSTRACTCOMMAND_H
#define ABSTRACTCOMMAND_H

#include <QObject>
#include <QIODevice>
#include <QDataStream>
#include <QDebug>
#include <QList>
#include <QVariant>

/**
 * @brief
 *
 */
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

/**
 * @brief
 *
 */
enum class CommandType {
    READ,
    WRITE,
    MODBUS,
    READWRITE,
};

/**
 * @brief
 *
 */
class AbstractCommand : public QObject {
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
explicit AbstractCommand(QObject *parent = nullptr);
    /**
     * @brief
     *
     * @return const QByteArray
     */
virtual const QByteArray &makeCommand() = 0;

    /**
     * @brief
     *
     * @return QVariant
     */
virtual QVariant getValue();
    /**
     * @brief
     *
     * @param v
     */
virtual void setValue(QVariant v);
    /**
     * @brief
     *
     * @return bool
     */
virtual bool isSuccess();

    /**
     * @brief
     *
     * @param data
     * @return bool
     */
virtual bool tryParse(const QByteArray &data);

public slots:
    /**
     * @brief
     *
     * @param data
     * @param regAddr
     */
virtual void processData(const QByteArray &data, quint16 regAddr);
    /**
     * @brief
     *
     * @param data
     */
virtual void processData(const QByteArray &data);
signals:
protected:
};

#endif // ABSTRACTCOMMAND_H
