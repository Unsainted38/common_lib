#ifndef ABSTRACTCOMMAND_H
#define ABSTRACTCOMMAND_H

#include <QObject>
#include <QIODevice>
#include <QDataStream>
#include <QDebug>
#include <QList>

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
    explicit AbstractCommand(ValueType value_type, CommandType cmd_type = CommandType::READWRITE, QObject *parent = nullptr);
    virtual const QByteArray &makeCommand() = 0;
    template <typename T> T getV() {
        switch(m_type) {
            case ValueType::QINT8:
                return static_cast<T>(m_val8);

            case ValueType::QUINT8:
                return static_cast<T>(m_valu8);

            case ValueType::QUINT16:
                return static_cast<T>(m_valu16);

            case ValueType::QUINT32:
                return static_cast<T>(m_valu32);

            case ValueType::QINT32:
                return static_cast<T>(m_val32);

            case ValueType::FLOAT:
                return static_cast<T>(m_valf);

            case ValueType::DOUBLE:
                return static_cast<T>(m_vald);
        }
    }
    template<typename T> void setV(const T &v) {
        switch(m_type) {
            case ValueType::QINT8:
                m_val8 = (qint8)v;
                break;

            case ValueType::QUINT8:
                m_valu8 = (quint8)v;
                break;

            case ValueType::QUINT16:
                m_valu16 = (quint16)v;
                break;

            case ValueType::QUINT32:
                m_valu32 = (quint32)v;
                break;

            case ValueType::QINT32:
                m_val32 = (qint32)v;
                break;

            case ValueType::FLOAT:
                m_valf = (float)v;
                break;

            case ValueType::DOUBLE:
                m_vald = (double)v;
                break;
        }
    }
    void setS(QString s) {
        m_valQStr = s;
    }
    QString getS() {
        return m_valQStr;
    }
    void setByteArray(QByteArray ba) {
        m_valQByteArray = ba;
    }
    QByteArray getByteArray() {
        return m_valQByteArray;
    }

    void setListQuint16(QList<quint16> list) {
        m_valListQuint16 = list;
    }
    QList<quint16> getListQuint16() {
        return m_valListQuint16;
    }
    void setListQuint8(QList<quint8> list) {
        m_valListQuint8 = list;
    }
    QList<quint8> getListQuint8() {
        return m_valListQuint8;
    }

    void setBool(bool b) {
        m_valBool = b;
    }

    bool getBool() {
        return m_valBool;
    }
public slots:
    virtual void processData(const QByteArray &data, quint16 regAddr);
signals:
protected:
    ValueType m_type;
    CommandType cmdType;
    qint8 m_val8 = 0;
    quint8 m_valu8 = 0;
    quint16 m_valu16 = 0;
    quint32 m_valu32 = 0;
    qint32 m_val32 = 0;
    QString m_valQStr = "";
    QByteArray m_valQByteArray = "";
    QList<quint16> m_valListQuint16;
    QList<quint8> m_valListQuint8;
    float m_valf = 0.0;
    double m_vald = 0.0;
    bool m_valBool = false;
};

#endif // ABSTRACTCOMMAND_H
