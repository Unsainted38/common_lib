#ifndef TCPTRANSPORT_H
#define TCPTRANSPORT_H

#include "abstract_network_transport.h"
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QQueue>
#include <QTimer>
#include <QDateTime>

/**
 * @brief Передаёт пакеты через TCP с очередью и автоматическим переподключением.
 */
class TcpTransport : public AbstractNetworkTransport {
    /**
     * @brief Хранит пакет и прогресс его постановки в транспорт.
     */
struct PendingPacket {
        quint64 id = 0; /**< Уникальный идентификатор пакета. */
        QByteArray data; /**< Текущее значение или полезная нагрузка. */
        qsizetype acceptedOffset = 0; /**< Количество байтов, принятых методом write(). */
        qsizetype confirmedOffset = 0; /**< Количество байтов, подтверждённых сигналом bytesWritten(). */
    };

    QTcpSocket *socket; /**< Хранит socket. */
    QHostAddress hostAddress; /**< Адрес host address. */
    quint16 port = 7777; /**< Номер локального или удалённого порта. */
    QHostAddress listenIp; /**< Хранит listen ip. */
    QString name = "tcp_client"; /**< Диагностическое имя объекта. */
    QMutex mutex; /**< Защищает очередь при доступе из разных потоков. */
    QQueue<PendingPacket> queue; /**< Очередь ожидающих отправки пакетов. */
    quint64 nextPacketId = 1; /**< Следующий идентификатор пакета. */
    QTimer *heartbeatTimer; /**< Таймер heartbeat timer. */
    QTimer *reconnectTimer; /**< Таймер reconnect timer. */
    bool connectedState = false; /**< Хранит connected state. */
    bool reconnectEnabled = true; /**< Хранит reconnect enabled. */
    QDateTime lastActivity; /**< Хранит last activity. */
    QByteArray heartbeatPacket; /**< Данные heartbeat packet. */

public:
    /**
     * @brief Передаёт пакеты через TCP с очередью и автоматическим переподключением.
     *
     * @param configPath Путь к INI-файлу конфигурации.
     * @param section Имя секции с параметрами объекта.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit TcpTransport(QString configPath, QString section, QObject *parent = nullptr);
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
     * @brief Закрывает транспорт и отключает автоматическое переподключение.
     *
     * @return true, если операция была принята к выполнению.
     */
bool close() override;
    /**
     * @brief Создаёт и настраивает объект ввода-вывода и таймеры транспорта.
     */
void setupTransport() override;
    /**
     * @brief Загружает параметры из указанной секции INI-файла.
     */
void loadConfig() override;
    /**
     * @brief Отправляет служебный пакет при активном соединении.
     */
void heartbeat() override;
    /**
     * @brief Открывает транспорт или запускает установление соединения.
     *
     * @return true, если операция была принята к выполнению.
     */
bool open() override;

signals:
    //void connectedChanged(bool);
private:
    /**
     * @brief Обновляет состояние после установления соединения.
     */
void onConnected();
    /**
     * @brief Сохраняет очередь и запускает восстановление соединения.
     */
void onDisconnected();
    /**
     * @brief Обрабатывает ошибку сокета и планирует переподключение.
     *
     * @param error Код ошибки сокета Qt.
     */
void onErrorOccured(QAbstractSocket::SocketError error);
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
    /**
     * @brief Считывает все доступные данные сокета.
     */
void onReadSocket();
    /**
     * @brief Останавливает таймеры активного соединения.
     */
void resetTimers();
    /**
     * @brief Планирует отложенную попытку переподключения.
     */
void scheduleReconnect();
    /**
     * @brief Повторно устанавливает соединение после разрыва.
     */
void reconnect();
public slots:
    //    void onConnected();
    //    void onDisconnected();
    //    void onErrorOccured(QAbstractSocket::SocketError error);
private slots:
    //    void processQueue() override;
    //    void onReadSocket();
    //    void resetTimers();
    //    void scheduleReconnect();
    //    void reconnect();
    //    void onConnected();
    //    void onDisconnected();
    //    void onErrorOccured(QAbstractSocket::SocketError error);
};

#endif // TCPTRANSPORT_H
