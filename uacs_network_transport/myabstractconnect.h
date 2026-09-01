#ifndef MYABSTRACTCONNECT_H
#define MYABSTRACTCONNECT_H

#include <QObject>
#include <QByteArray>

/**
 * @brief Устаревший абстрактный интерфейс последовательного соединения.
 */
class MyAbstractConnect : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Устаревший абстрактный интерфейс последовательного соединения.
     *
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit MyAbstractConnect(QObject *parent = nullptr);

    /**
     * @brief Добавляет данные в очередь на отправку.
     *
     * @param data Входные данные или полезная нагрузка ответа.
     */
virtual void writeData(const QByteArray &data) = 0;


public slots:
    /**
     * @brief Считывает входные данные и передаёт их обработчикам.
     */
virtual void onReadyRead() = 0;
    /**
     * @brief Завершает текущий пакет после полной передачи его байтов.
     *
     * @param bytes Количество байтов, подтверждённых устройством ввода-вывода.
     */
virtual void handleBytesWritten(qint64 bytes) = 0;
    /**
     * @brief Продолжает отправку текущего пакета.
     */
virtual void writeNext() = 0;

signals:
    /**
     * @brief Передаёт принятые байты обработчикам протокола.
     */
void readyToProcessData(QByteArray);
};

#endif // MYABSTRACTCONNECT_H
