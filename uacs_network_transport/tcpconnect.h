#ifndef TCPCONNECT_H
#define TCPCONNECT_H

#include <QTcpSocket>
#include <QDebug>
#include <QQueue>
#include <QTimer>

#include "myabstractconnect.h"

/**
 * @brief Устаревший TCP-адаптер с безопасной очередью записи.
 */
class TcpConnect : public MyAbstractConnect
{
public:
    /**
     * @brief Устаревший TCP-адаптер с безопасной очередью записи.
     *
     * @param ip IP-адрес удалённого узла.
     * @param port TCP-порт удалённого узла.
     */
    TcpConnect(QString ip, quint16 port);
    QString ipConnect; /**< Хранит ip connect. */
    quint16 portConnect; /**< Хранит port connect. */

    QTcpSocket *socket; /**< Хранит socket. */

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

#endif // TCPCONNECT_H
