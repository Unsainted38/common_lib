#ifndef TCPCONNECT_H
#define TCPCONNECT_H

#include <QTcpSocket>
#include <QDebug>
#include <QQueue>
#include <QTimer>

#include "myabstractconnect.h"

class TcpConnect : public MyAbstractConnect
{
public:
    TcpConnect(QString ip, quint16 port);
    QString ipConnect;
    quint16 portConnect;

    QTcpSocket *socket;

    QQueue<QByteArray> packetQueue;
    QByteArray currentPacket;
    qsizetype currentOffset = 0;
    bool isBusy = false;

    void writeData(const QByteArray &data) override;

public slots:
    void onReadyRead() override;
    void handleBytesWritten(qint64 bytes) override;
    void writeNext() override;
};

#endif // TCPCONNECT_H
