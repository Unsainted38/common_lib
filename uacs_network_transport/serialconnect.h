#ifndef SERIALCONNECT_H
#define SERIALCONNECT_H

//#include <QtSerialPort/QSerialPort>
#include <QSerialPort>
#include <QDebug>
#include <QQueue>
#include <QTimer>

#include "myabstractconnect.h"


class SerialConnect : public MyAbstractConnect
{
public:
    SerialConnect(QString nameport, int baundrate, int databits, QString parity, int stopbit, QString flowcontrol);

    QString np;
    int br;
    int db;
    QString pt;
    int sb;
    QString fc;

    QSerialPort *serial;
    QQueue<QByteArray> packetQueue;
    QByteArray currentPacket;
    bool isBusy = false;


    void writeData(const QByteArray &data);

public slots:
    void onReadyRead() override;
    void handleBytesWritten(qint64 bytes) override;
    void writeNext() override;
};

#endif // SERIALCONNECT_H
