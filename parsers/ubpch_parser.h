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
    QByteArray unpack(const QByteArray &frame);
signals:
    void dataReady(QByteArray, quint16);
    void statusOnline();
    void lastAnswer(QByteArray);
private:
    QByteArray m_buffer;

public slots:
    bool parseReply(const QByteArray &reply);
};

#endif // UBPCHPARSER_H
