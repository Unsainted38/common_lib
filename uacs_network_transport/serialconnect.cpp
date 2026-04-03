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
    //connect(serial, SIGNAL(bytesWritten(qint64)), this, SLOT(handleBytesWritten(qint64)));
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
    if (packetQueue.isEmpty()) return;
    currentPacket = packetQueue.dequeue();
    serial->write(currentPacket);
    isBusy = true;
}

void SerialConnect::onReadyRead()
{
    QByteArray data = serial->readAll();
    isBusy = false;
    emit readyToProcessData(data);
    if (!packetQueue.isEmpty())
    {
        QTimer::singleShot(10, this, SLOT(writeNext()));
    }
}

void SerialConnect::handleBytesWritten(qint64 bytes)
{
    Q_UNUSED(bytes);
    // When finished writing all bytes of currentPacket
    if (serial->bytesToWrite() == 0) {
        isBusy = false;
        if (!packetQueue.isEmpty()) {
            writeNext();
        }
    }
}

void SerialConnect::writeData(const QByteArray &data)
{
    // serial->write(data);
    // serial->flush();
    // serial->waitForBytesWritten(40);
    packetQueue.enqueue(data);
    if (!isBusy) writeNext();

}
