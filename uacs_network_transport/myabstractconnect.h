#ifndef MYABSTRACTCONNECT_H
#define MYABSTRACTCONNECT_H

#include <QObject>
#include <QByteArray>

class MyAbstractConnect : public QObject
{
    Q_OBJECT
public:
    explicit MyAbstractConnect(QObject *parent = nullptr);

    virtual void writeData(const QByteArray &data) = 0;


public slots:
    virtual void onReadyRead() = 0;
    virtual void handleBytesWritten(qint64 bytes) = 0;
    virtual void writeNext() = 0;

signals:
    void readyToProcessData(QByteArray);
};

#endif // MYABSTRACTCONNECT_H
