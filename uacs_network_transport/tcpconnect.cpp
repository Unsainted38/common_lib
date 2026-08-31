#include "tcpconnect.h"

TcpConnect::TcpConnect(QString ip, quint16 port) :
    ipConnect(ip),
    portConnect(port)
{

    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, &QTcpSocket::bytesWritten,
            this, &TcpConnect::handleBytesWritten);
    connect(socket, &QTcpSocket::connected,
            this, &TcpConnect::writeNext);
    connect(socket, &QTcpSocket::disconnected, this, [this]() {
        // В новом TCP-потоке частично переданный кадр нужно начать заново.
        currentPacket.clear();
        currentOffset = 0;
        isBusy = false;
    });
    socket->connectToHost(ipConnect, portConnect);
}

void TcpConnect::writeData(const QByteArray &data)
{
    if (data.isEmpty()) {
        return;
    }

    packetQueue.enqueue(data);
    writeNext();
}

void TcpConnect::onReadyRead()
{
    QByteArray data = socket->readAll();
    emit readyToProcessData(data);
}

void TcpConnect::handleBytesWritten(qint64 bytes)
{
    Q_UNUSED(bytes);

    if (isBusy && currentOffset == currentPacket.size() &&
        socket->bytesToWrite() == 0) {
        if (!packetQueue.isEmpty()) {
            packetQueue.dequeue();
        }
        currentPacket.clear();
        currentOffset = 0;
        isBusy = false;
    }

    writeNext();
}

void TcpConnect::writeNext()
{
    if (socket->state() != QAbstractSocket::ConnectedState ||
        packetQueue.isEmpty()) {
        return;
    }

    if (!isBusy) {
        currentPacket = packetQueue.head();
        currentOffset = 0;
        isBusy = true;
    }

    if (currentOffset >= currentPacket.size()) {
        return;
    }

    const qint64 written = socket->write(
        currentPacket.constData() + currentOffset,
        currentPacket.size() - currentOffset);
    if (written > 0) {
        currentOffset += static_cast<qsizetype>(written);
    } else if (written == 0) {
        QTimer::singleShot(10, this, &TcpConnect::writeNext);
    } else {
        qWarning() << "TCP write failed:" << socket->errorString();
        QTimer::singleShot(100, this, &TcpConnect::writeNext);
    }
}
