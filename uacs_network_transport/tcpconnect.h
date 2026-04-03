#ifndef TCPCONNECT_H
#define TCPCONNECT_H

#include <QTcpSocket>
#include <QDebug>
#include <QQueue>
#include <QTimer>
#include <QElapsedTimer>

#include "myabstractconnect.h"

class TcpConnect : public MyAbstractConnect
{
public:
    TcpConnect(QString ip, quint16 port);
    QString ipConnect;
    quint16 portConnect;

    QTcpSocket *socket;

    QElapsedTimer *elapsedTimer;
    QQueue<QByteArray> packetQueue;
    QByteArray currentPacket;
    bool isBusy = false;

    void writeData(const QByteArray &data) override;

public slots:
    void onReadyRead() override;
    void handleBytesWritten(qint64 bytes);
    void writeNext();
};

#endif // TCPCONNECT_H
