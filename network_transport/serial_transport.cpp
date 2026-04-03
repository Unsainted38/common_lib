#include "serial_transport.h"

SerialTransport::SerialTransport(QString configPath, QString section, QObject *parent)
    : AbstractNetworkTransport(configPath, section) {
    loadConfig();
    serial = new QSerialPort(this);
    setupTransport();
    open();
    connect(serial, SIGNAL(readyRead()), this, SLOT(onSerialRead()));
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
    QMutexLocker locker(&mutex);
    queue.enqueue(packet);
    QMetaObject::invokeMethod(this, "processQueue", Qt::QueuedConnection);
    return true;
}

bool SerialTransport::open() {
    if(!serial->open(QSerialPort::ReadWrite)) {
        qDebug() << "error while opening serial port: " << portName;
        qDebug() << serial->errorString();
        return false;
    }

    qDebug() << "opened serial port: " << portName;
    return true;
}

bool SerialTransport::close() {
    serial->close();
    return true;
}

void SerialTransport::heartbeat() {

}

void SerialTransport::onSerialRead() {
    emit translateData(serial->readAll());
}

void SerialTransport::processQueue() {
    QMutexLocker locker(&mutex);

    if(!queue.isEmpty()) {
        QByteArray packet = queue.dequeue();
        int writeCount = serial->write(packet);
        serial->flush();
        QString wErr = "w: " + QString::number(writeCount);

        if(writeCount < packet.length()) {
            emit translateError(wErr, WRITE_ERROR);
        } else {
            emit translateError(wErr, WRITE_OK);
        }
    }
}
