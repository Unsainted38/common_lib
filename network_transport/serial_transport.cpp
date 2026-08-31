#include "serial_transport.h"
#include <QTimer>

SerialTransport::SerialTransport(QString configPath, QString section, QObject *parent)
    : AbstractNetworkTransport(configPath, section, parent) {
    loadConfig();
    serial = new QSerialPort(this);
    setupTransport();
    open();
    connect(serial, SIGNAL(readyRead()), this, SLOT(onSerialRead()));
    connect(serial, &QSerialPort::bytesWritten,
            this, &SerialTransport::onBytesWritten);
}

void SerialTransport::loadConfig() {
    QSettings settings(m_configPath, QSettings::IniFormat);
    settings.beginGroup(m_section);
    name = settings.value("Name", "serialport").toString();
    portName = settings.value("PortName", "/dev/ttyS3").toString();
    baud = settings.value("BaudRate", QSerialPort::Baud115200).toUInt();
    dataBits = settings.value("DataBits", QSerialPort::Data8).toUInt();
    parity = settings.value("Parity", QSerialPort::NoParity).toInt();
    stopBits = settings.value("StopBits", QSerialPort::TwoStop).toInt();
    flowControl = settings.value("FlowControl", QSerialPort::NoFlowControl).toInt();
    settings.endGroup();
}

void SerialTransport::setupTransport() {
    serial->setBaudRate(baud);
    serial->setPortName(portName);
    serial->setDataBits((QSerialPort::DataBits)dataBits);
    serial->setStopBits((QSerialPort::StopBits)stopBits);
    serial->setFlowControl((QSerialPort::FlowControl)flowControl);
    serial->setParity((QSerialPort::Parity)parity);
    qDebug() << m_configPath
             << "Name" << name
             << "Port name: " << portName
             << "BaudRate: " << baud
             << "DataBits: " << dataBits
             << "Parity: " << parity
             << "StopBits: " << stopBits;
}


bool SerialTransport::write(const QByteArray &packet) {
    return writeTracked(packet) != 0;
}

quint64 SerialTransport::writeTracked(const QByteArray &packet) {
    if (packet.isEmpty()) {
        return 0;
    }

    quint64 packetId = 0;
    {
        QMutexLocker locker(&mutex);
        packetId = nextPacketId++;
        if (nextPacketId == 0) {
            nextPacketId = 1;
        }
        queue.enqueue({packetId, packet, 0, 0});
    }

    QMetaObject::invokeMethod(
        this,
        [this]() { processQueue(); },
        Qt::QueuedConnection);
    return packetId;
}

bool SerialTransport::open() {
    if(!serial->open(QSerialPort::ReadWrite)) {
        qDebug() << "error while opening serial port: " << portName;
        qDebug() << serial->errorString();
        return false;
    }

    qDebug() << "opened serial port: " << portName;

    QMetaObject::invokeMethod(
        this,
        [this]() { processQueue(); },
        Qt::QueuedConnection);
    return true;
}

bool SerialTransport::close() {
    {
        QMutexLocker locker(&mutex);
        if (!queue.isEmpty()) {
            queue.head().acceptedOffset = 0;
            queue.head().confirmedOffset = 0;
        }
    }
    serial->close();
    return true;
}

void SerialTransport::heartbeat() {

}

void SerialTransport::onSerialRead() {
    emit translateData(serial->readAll());
}

void SerialTransport::processQueue() {
    QString writeResult;
    bool writeFailed = false;
    bool retryAfterDelay = false;

    {
        QMutexLocker locker(&mutex);

        if (queue.isEmpty() || !serial->isOpen()) {
            return;
        }

        PendingPacket &pending = queue.head();
        const qsizetype remaining =
            pending.data.size() - pending.acceptedOffset;

        if (remaining == 0) {
            return;
        }

        const qint64 writeCount = serial->write(
            pending.data.constData() + pending.acceptedOffset,
            remaining);
        writeResult = "w: " + QString::number(writeCount);

        if (writeCount < 0) {
            writeFailed = true;
        } else {
            pending.acceptedOffset += static_cast<qsizetype>(writeCount);
            retryAfterDelay = writeCount == 0;
        }
    }

    serial->flush();

    if (writeFailed) {
        emit translateError(writeResult, WRITE_ERROR);
        QTimer::singleShot(100, this, [this]() { processQueue(); });
        return;
    }

    if (retryAfterDelay) {
        QTimer::singleShot(50, this, [this]() { processQueue(); });
    }
}

void SerialTransport::onBytesWritten(qint64 count)
{
    quint64 acceptedPacketId = 0;
    QByteArray acceptedPacket;
    bool processMore = false;

    {
        QMutexLocker locker(&mutex);
        if (queue.isEmpty() || count <= 0) {
            return;
        }

        PendingPacket &pending = queue.head();
        const qsizetype unconfirmed =
            pending.acceptedOffset - pending.confirmedOffset;
        const qsizetype confirmedNow = qMin(
            static_cast<qsizetype>(count), unconfirmed);
        pending.confirmedOffset += confirmedNow;

        if (pending.acceptedOffset == pending.data.size() &&
            pending.confirmedOffset == pending.data.size()) {
            acceptedPacketId = pending.id;
            acceptedPacket = pending.data;
            queue.dequeue();
            processMore = !queue.isEmpty();
        } else {
            processMore = pending.acceptedOffset < pending.data.size();
        }
    }

    if (acceptedPacketId != 0) {
        emit translateError(
            "w: " + QString::number(acceptedPacket.size()), WRITE_OK);
        emit packetAccepted(acceptedPacketId, acceptedPacket);
    }

    if (processMore) {
        QMetaObject::invokeMethod(
            this,
            [this]() { processQueue(); },
            Qt::QueuedConnection);
    }
}
