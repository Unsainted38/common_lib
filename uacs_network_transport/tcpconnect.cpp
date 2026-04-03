#include "tcpconnect.h"

TcpConnect::TcpConnect(QString ip, quint16 port) :
    ipConnect(ip),
    portConnect(port)
{

    elapsedTimer = new QElapsedTimer();
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    socket->connectToHost(ipConnect, portConnect);
}

void TcpConnect::writeData(const QByteArray &data)
{
    socket->write(data);
    // if(socket->isOpen())
    // {
    //     packetQueue.enqueue(data);
    //     if (!isBusy || (elapsedTimer->elapsed() >= 50)) {
    //         writeNext();
    //         elapsedTimer->restart();
    //     }
    // }
}

void TcpConnect::onReadyRead()
{
    QByteArray data = socket->readAll();
    isBusy = false;
    emit readyToProcessData(data);
    if (!packetQueue.isEmpty())
    {
        QTimer::singleShot(10, this, SLOT(writeNext()));
    }
}

void TcpConnect::handleBytesWritten(qint64 bytes)
{
    Q_UNUSED(bytes);
    // When finished writing all bytes of currentPacket
    if (socket->bytesToWrite() == 0) {
        isBusy = false;
        if (!packetQueue.isEmpty()) {
            writeNext();
        }
    }
}

void TcpConnect::writeNext()
{
    if (packetQueue.isEmpty()) return;
    currentPacket = packetQueue.dequeue();
    socket->write(currentPacket);
    isBusy = true;
}
