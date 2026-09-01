#ifndef SERIALTRANSPORT_H
#define SERIALTRANSPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMutex>
#include <QQueue>

#include "abstract_network_transport.h"

/**
 * @brief
 *
 */
class SerialTransport : public AbstractNetworkTransport {
    Q_OBJECT
    /**
     * @brief
     *
     */
struct PendingPacket {
        quint64 id = 0; /**< TODO: describe */
        QByteArray data; /**< TODO: describe */
        qsizetype acceptedOffset = 0; /**< TODO: describe */
        qsizetype confirmedOffset = 0; /**< TODO: describe */
    };

public:
    /**
     * @brief
     *
     * @param configPath
     * @param section
     * @param parent
     */
explicit SerialTransport(QString configPath, QString section, QObject *parent = nullptr);

public:
    /**
     * @brief
     *
     */
void loadConfig() override;
    /**
     * @brief
     *
     */
void setupTransport() override;
    /**
     * @brief
     *
     * @param packet
     * @return bool
     */
bool write(const QByteArray &packet) override;
    /**
     * @brief
     *
     * @param packet
     * @return quint64
     */
quint64 writeTracked(const QByteArray &packet) override;
    /**
     * @brief
     *
     * @return bool
     */
bool open() override;
    /**
     * @brief
     *
     * @return bool
     */
bool close() override;
    /**
     * @brief
     *
     */
void heartbeat() override;
signals:
private:
    QMutex mutex; /**< TODO: describe */
    QQueue<PendingPacket> queue; /**< TODO: describe */
    quint64 nextPacketId = 1; /**< TODO: describe */
    QString portName; /**< TODO: describe */
    QString name; /**< TODO: describe */
    int baud; /**< TODO: describe */
    int dataBits; /**< TODO: describe */
    int parity; /**< TODO: describe */
    int flowControl; /**< TODO: describe */
    int stopBits; /**< TODO: describe */

    QSerialPort *serial; /**< TODO: describe */
private slots:
    /**
     * @brief
     *
     */
void onSerialRead();
    /**
     * @brief
     *
     */
void processQueue() override;
    /**
     * @brief
     *
     * @param count
     */
void onBytesWritten(qint64 count);
};

#endif // SERIALTRANSPORT_H
