#ifndef UBPCHTRANSPORT_H
#define SERIALTRANSPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMutex>
#include <QQueue>

#include "abstract_network_transport.h"

class SerialTransport : public AbstractNetworkTransport {
    Q_OBJECT
public:
    explicit SerialTransport(QString configPath, QString section, QObject *parent = nullptr);

public:
    void loadConfig() override;
    void setupTransport() override;
    bool write(const QByteArray &packet) override;
    bool open() override;
    bool close() override;
    void heartbeat() override;
signals:
private:
    QMutex mutex;
    QQueue<QByteArray> queue;
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
};

#endif // SERIALTRANSPORT_H
