#ifndef SERIALCONNECT_H
#define SERIALCONNECT_H

//#include <QtSerialPort/QSerialPort>
#include <QSerialPort>
#include <QDebug>
#include <QQueue>
#include <QTimer>

#include "myabstractconnect.h"


/**
 * @brief Устаревший адаптер QSerialPort с безопасной очередью записи.
 */
class SerialConnect : public MyAbstractConnect
{
public:
    /**
     * @brief Устаревший адаптер QSerialPort с безопасной очередью записи.
     *
     * @param nameport Имя последовательного порта.
     * @param baundrate Скорость последовательного порта.
     * @param databits Количество битов данных.
     * @param parity Режим контроля чётности.
     * @param stopbit Количество стоп-битов.
     * @param flowcontrol Режим управления потоком.
     */
    SerialConnect(QString nameport, int baundrate, int databits, QString parity, int stopbit, QString flowcontrol);

    QString np; /**< Хранит np. */
    int br; /**< Хранит br. */
    int db; /**< Хранит db. */
    QString pt; /**< Хранит pt. */
    int sb; /**< Хранит sb. */
    QString fc; /**< Хранит fc. */

    QSerialPort *serial; /**< Хранит serial. */
    QQueue<QByteArray> packetQueue; /**< Хранит packet queue. */
    QByteArray currentPacket; /**< Пакет, отправляемый в настоящий момент. */
    qsizetype currentOffset = 0; /**< Количество байтов текущего пакета, принятых устройством ввода-вывода. */
    bool isBusy = false; /**< Признак активной записи пакета. */


    /**
     * @brief Добавляет данные в очередь на отправку.
     *
     * @param data Входные данные или полезная нагрузка ответа.
     */
void writeData(const QByteArray &data) override;

public slots:
    /**
     * @brief Считывает входные данные и передаёт их обработчикам.
     */
void onReadyRead() override;
    /**
     * @brief Завершает текущий пакет после полной передачи его байтов.
     *
     * @param bytes Количество байтов, подтверждённых устройством ввода-вывода.
     */
void handleBytesWritten(qint64 bytes) override;
    /**
     * @brief Продолжает отправку текущего пакета.
     */
void writeNext() override;
};

#endif // SERIALCONNECT_H
