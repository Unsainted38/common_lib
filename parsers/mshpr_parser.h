#ifndef MSHPRPARSER_H
#define MSHPRPARSER_H

#include <QObject>
#include <QDataStream>
#include <QIODevice>
#include <QMap>
#include <QByteArray>
#include <QStringList>

class MShPRParser : public QObject {
    Q_OBJECT
public:
    explicit MShPRParser(QObject *parent = nullptr);

signals:
    void dataReady(QString, QMap<QString, int>);
    void lastAnswer(QByteArray);
public slots:
    bool parseReply(const QByteArray &reply);
private:
    QByteArray m_buffer;

};

#endif // MSHPRPARSER_H
