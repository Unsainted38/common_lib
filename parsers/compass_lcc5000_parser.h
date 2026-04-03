#ifndef COMPASSLCC5000PARSER_H
#define COMPASSLCC5000PARSER_H

#include <QObject>
#include <QDataStream>
#include <QIODevice>
#include "algorithm/crc8.h"

class CompassLCC5000Parser : public QObject {
    Q_OBJECT
public:
    explicit CompassLCC5000Parser(QObject *parent = nullptr);
    explicit CompassLCC5000Parser(quint8 deviceAddr, QObject *parent = nullptr);
public slots:
    bool parseReply(const QByteArray &data);

signals:
    void dataReady(QByteArray, quint8);
    void lastAnswer(QByteArray);
private:
    QByteArray m_buffer;
    quint8 m_deviceAddr;
};

#endif // COMPASSLCC5000PARSER_H
