#ifndef SERIALCONNECT_H
#define SERIALCONNECT_H

//#include <QtSerialPort/QSerialPort>
#include <QSerialPort>
#include <QDebug>
#include <QQueue>
#include <QTimer>

#include "myabstractconnect.h"


/**
 * @brief
 *
 */
class SerialConnect : public MyAbstractConnect
{
public:
    /**
     * @brief
     *
     * @param nameport
     * @param baundrate
     * @param databits
     * @param parity
     * @param stopbit
     * @param flowcontrol
     */
    SerialConnect(QString nameport, int baundrate, int databits, QString parity, int stopbit, QString flowcontrol);

    QString np; /**< TODO: describe */
    int br; /**< TODO: describe */
    int db; /**< TODO: describe */
    QString pt; /**< TODO: describe */
    int sb; /**< TODO: describe */
    QString fc; /**< TODO: describe */

    QSerialPort *serial; /**< TODO: describe */
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

#endif // SERIALCONNECT_H
