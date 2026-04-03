#ifndef ABSTRACTCOMMAND_H
#define ABSTRACTCOMMAND_H

#include <QObject>
#include <QIODevice>
#include <QDataStream>
#include <QDebug>

enum ValueType {
    QINT8 = 7,
    QUINT8 = 8,
    QUINT16 = 16,
    QUINT32 = 32,
    QINT32 = 31,
    QSTRING = 25,
    FLOAT = 26,
    DOUBLE = 27
};

class AbstractCommand : public QObject {
    Q_OBJECT
public:
    explicit AbstractCommand(ValueType type, QObject *parent = nullptr);
    virtual const QByteArray &makeReadCommand() = 0;
    virtual const QByteArray &makeWriteCommand() = 0;
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
public slots:
    virtual void processData(const QByteArray &data, quint16 regAddr);
signals:
protected:
    ValueType m_type;
    qint8 m_val8 = 0;
    quint8 m_valu8 = 0;
    quint16 m_valu16 = 0;
    quint32 m_valu32 = 0;
    qint32 m_val32 = 0;
    QString m_valQStr = "";
    float m_valf = 0.0;
    double m_vald = 0.0;
};

#endif // ABSTRACTCOMMAND_H
