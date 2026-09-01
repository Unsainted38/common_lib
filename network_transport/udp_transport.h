#ifndef UDP_TRANSPORT_H
#define UDP_TRANSPORT_H

#include <QObject>
#include "abstract_network_transport.h"
#include <QUdpSocket>
#include <QQueue>
#include <QTimer>
#include <QDateTime>


/**
 * @brief Передаёт атомарные UDP-дейтаграммы с очередью и переподключением.
 */
class UdpTransport : public AbstractNetworkTransport
{
    Q_OBJECT
    /**
     * @brief Хранит пакет и прогресс его постановки в транспорт.
     */
struct PendingPacket {
        quint64 id = 0; /**< Уникальный идентификатор пакета. */
        QByteArray data; /**< Текущее значение или полезная нагрузка. */
    };

    QUdpSocket *socket; /**< Хранит socket. */
    QHostAddress hostAddress; /**< Адрес host address. */
    quint16 port = 7777; /**< Номер локального или удалённого порта. */
    QHostAddress listenIp; /**< Хранит listen ip. */
    QString name = "udp_client"; /**< Диагностическое имя объекта. */
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
     * @brief Передаёт атомарные UDP-дейтаграммы с очередью и переподключением.
     *
     * @param configPath Путь к INI-файлу конфигурации.
     * @param section Имя секции с параметрами объекта.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit UdpTransport(QString configPath, QString section, QObject *parent = nullptr);

    // AbstractNetworkTransport interface
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
     * @brief Открывает транспорт или запускает установление соединения.
     *
     * @return true, если операция была принята к выполнению.
     */
bool open() override;
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
     * @brief Отправляет служебный пакет при активном соединении.
     */
void heartbeat() override;

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
};

#endif // UDP_TRANSPORT_H
