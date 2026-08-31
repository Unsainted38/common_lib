#ifndef UBPCHPARSER_H
#define UBPCHPARSER_H

#include <QObject>
#include <QDataStream>
#include <QByteArray>
#include <QIODevice>

class UBPChParser : public QObject {
    Q_OBJECT
public:
    explicit UBPChParser(QObject *parent = 0);
    UBPChParser(quint8 deviceAddr, quint8 masterAddr,
                QObject *parent = nullptr);
    QByteArray unpack(const QByteArray &frame);
signals:
    void dataReady(QByteArray, quint16);
    void statusOnline();
    void lastAnswer(QByteArray);
private:
    QByteArray m_buffer;
    quint8 m_deviceAddr = 0;
    quint8 m_masterAddr = 0;
    bool m_filterAddresses = false;

public slots:
    bool parseReply(const QByteArray &reply);
};

#endif // UBPCHPARSER_H
