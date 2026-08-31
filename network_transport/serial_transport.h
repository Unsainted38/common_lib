#ifndef SERIALTRANSPORT_H
#define SERIALTRANSPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMutex>
#include <QQueue>

#include "abstract_network_transport.h"

class SerialTransport : public AbstractNetworkTransport {
    Q_OBJECT
    struct PendingPacket {
        quint64 id = 0;
        QByteArray data;
        qsizetype acceptedOffset = 0;
        qsizetype confirmedOffset = 0;
    };

public:
    explicit SerialTransport(QString configPath, QString section, QObject *parent = nullptr);

public:
    void loadConfig() override;
    void setupTransport() override;
    bool write(const QByteArray &packet) override;
    quint64 writeTracked(const QByteArray &packet) override;
    bool open() override;
    bool close() override;
    void heartbeat() override;
signals:
private:
    QMutex mutex;
    QQueue<PendingPacket> queue;
    quint64 nextPacketId = 1;
    QString portName;
    QString name;
    int baud;
    int dataBits;
    int parity;
    int flowControl;
    int stopBits;

    QSerialPort *serial;
private slots:
    void onSerialRead();
    void processQueue() override;
    void onBytesWritten(qint64 count);
};

#endif // SERIALTRANSPORT_H
