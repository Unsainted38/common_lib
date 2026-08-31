#include "serialconnect.h"

SerialConnect::SerialConnect(QString nameport, int baundrate, int databits, QString parity, int stopbit, QString flowcontrol) :
    np(nameport),
    br(baundrate),
    db(databits),
    pt(parity),
    sb(stopbit),
    fc(flowcontrol)
{
    serial = new QSerialPort(this);
    connect(serial, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(serial, &QSerialPort::bytesWritten,
            this, &SerialConnect::handleBytesWritten);
    serial->setPortName(np);
    serial->setBaudRate(br);

    if (db == 5) serial->setDataBits(QSerialPort::Data5);
    if (db == 6) serial->setDataBits(QSerialPort::Data6);
    if (db == 7) serial->setDataBits(QSerialPort::Data7);
    if (db == 8) serial->setDataBits(QSerialPort::Data8);

    if (pt == "NoParity") serial->setParity(QSerialPort::NoParity);
    if (pt == "OddParity") serial->setParity(QSerialPort::OddParity);
    if (pt == "MarkParity") serial->setParity(QSerialPort::MarkParity);
    if (pt == "EvenParity") serial->setParity(QSerialPort::EvenParity);

    //if (stopbit == 0) serial->setStopBits(QSerialPort::UnknownStopBits);
    if (sb == 1) serial->setStopBits(QSerialPort::OneStop);
    if (sb == 2) serial->setStopBits(QSerialPort::TwoStop);

    if (fc == "NoFlowControl") serial->setFlowControl(QSerialPort::NoFlowControl);
    //if (flowcontrol == "UnknownFlowControl") serial->setFlowControl(QSerialPort::UnknownFlowControl);

    if(serial->open(QIODevice::ReadWrite))
    {
       qDebug() << "[SerialPort] -> " + nameport + " successeful open!";
    }
    else
    {
       qDebug() << "[ERROR] -> " + nameport + " don't open!";
    }


}

void SerialConnect::writeNext()
{
    if (!serial->isOpen() || packetQueue.isEmpty()) {
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

    const qint64 written = serial->write(
        currentPacket.constData() + currentOffset,
        currentPacket.size() - currentOffset);
    if (written > 0) {
        currentOffset += static_cast<qsizetype>(written);
    } else if (written == 0) {
        QTimer::singleShot(10, this, &SerialConnect::writeNext);
    } else {
        qWarning() << "Serial write failed:" << serial->errorString();
        QTimer::singleShot(100, this, &SerialConnect::writeNext);
    }
}

void SerialConnect::onReadyRead()
{
    QByteArray data = serial->readAll();
    emit readyToProcessData(data);
}

void SerialConnect::handleBytesWritten(qint64 bytes)
{
    Q_UNUSED(bytes);
    if (isBusy && currentOffset == currentPacket.size() &&
        serial->bytesToWrite() == 0) {
        if (!packetQueue.isEmpty()) {
            packetQueue.dequeue();
        }
        currentPacket.clear();
        currentOffset = 0;
        isBusy = false;
    }

    writeNext();
}

void SerialConnect::writeData(const QByteArray &data)
{
    if (data.isEmpty()) {
        return;
    }

    packetQueue.enqueue(data);
    writeNext();

}
