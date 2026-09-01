#ifndef SERIALTRANSPORT_H
#define SERIALTRANSPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMutex>
#include <QQueue>

#include "abstract_network_transport.h"

/**
 * @brief Передаёт пакеты через QSerialPort с поддержкой частичной записи.
 */
class SerialTransport : public AbstractNetworkTransport {
    Q_OBJECT
    /**
     * @brief Хранит пакет и прогресс его постановки в транспорт.
     */
struct PendingPacket {
        quint64 id = 0; /**< Уникальный идентификатор пакета. */
        QByteArray data; /**< Текущее значение или полезная нагрузка. */
        qsizetype acceptedOffset = 0; /**< Количество байтов, принятых методом write(). */
        qsizetype confirmedOffset = 0; /**< Количество байтов, подтверждённых сигналом bytesWritten(). */
    };

public:
    /**
     * @brief Передаёт пакеты через QSerialPort с поддержкой частичной записи.
     *
     * @param configPath Путь к INI-файлу конфигурации.
     * @param section Имя секции с параметрами объекта.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit SerialTransport(QString configPath, QString section, QObject *parent = nullptr);

public:
    /**
     * @brief Загружает параметры из указанной секции INI-файла.
     */
void loadConfig() override;
    /**
     * @brief Создаёт и настраивает объект ввода-вывода и таймеры транспорта.
     */
void setupTransport() override;
    /**
     * @brief Ставит пакет в очередь на отправку.
     *
     * @param packet Пакет данных для отправки.
     * @return true, если операция была принята к выполнению.
     */
bool write(const QByteArray &packet) override;
    /**
     * @brief Ставит пакет в очередь и возвращает его уникальный идентификатор.
     *
     * @param packet Пакет данных для отправки.
     * @return Идентификатор пакета или 0 при отказе постановки в очередь.
     */
quint64 writeTracked(const QByteArray &packet) override;
    /**
     * @brief Открывает транспорт или запускает установление соединения.
     *
     * @return true, если операция была принята к выполнению.
     */
bool open() override;
    /**
     * @brief Закрывает транспорт и отключает автоматическое переподключение.
     *
     * @return true, если операция была принята к выполнению.
     */
bool close() override;
    /**
     * @brief Отправляет служебный пакет при активном соединении.
     */
void heartbeat() override;
signals:
private:
    QMutex mutex; /**< Защищает очередь при доступе из разных потоков. */
    QQueue<PendingPacket> queue; /**< Очередь ожидающих отправки пакетов. */
    quint64 nextPacketId = 1; /**< Следующий идентификатор пакета. */
    QString portName; /**< Хранит port name. */
    QString name; /**< Диагностическое имя объекта. */
    int baud; /**< Хранит baud. */
    int dataBits; /**< Хранит data bits. */
    int parity; /**< Хранит parity. */
    int flowControl; /**< Хранит flow control. */
    int stopBits; /**< Хранит stop bits. */

    QSerialPort *serial; /**< Хранит serial. */
private slots:
    /**
     * @brief Считывает все доступные байты последовательного порта.
     */
void onSerialRead();
    /**
     * @brief Продолжает отправку первого пакета очереди с учётом частичной записи.
     */
void processQueue() override;
    /**
     * @brief Обновляет подтверждённый прогресс записи и продолжает очередь.
     *
     * @param count Количество подтверждённых байтов.
     */
void onBytesWritten(qint64 count);
};

#endif // SERIALTRANSPORT_H
