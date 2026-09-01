#ifndef TCPCONNECT_H
#define TCPCONNECT_H

#include <QTcpSocket>
#include <QDebug>
#include <QQueue>
#include <QTimer>

#include "myabstractconnect.h"

/**
 * @brief
 *
 */
class TcpConnect : public MyAbstractConnect
{
public:
    /**
     * @brief
     *
     * @param ip
     * @param port
     */
    TcpConnect(QString ip, quint16 port);
    QString ipConnect; /**< TODO: describe */
    quint16 portConnect; /**< TODO: describe */

    QTcpSocket *socket; /**< TODO: describe */

    QQueue<QByteArray> packetQueue; /**< TODO: describe */
    QByteArray currentPacket; /**< TODO: describe */
    qsizetype currentOffset = 0; /**< TODO: describe */
    bool isBusy = false; /**< TODO: describe */

    /**
     * @brief
     *
     * @param data
     */
void writeData(const QByteArray &data) override;

public slots:
    /**
     * @brief
     *
     */
void onReadyRead() override;
    /**
     * @brief
     *
     * @param bytes
     */
void handleBytesWritten(qint64 bytes) override;
    /**
     * @brief
     *
     */
void writeNext() override;
};

#endif // TCPCONNECT_H
