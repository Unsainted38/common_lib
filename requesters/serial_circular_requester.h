#ifndef SERIALCIRCULARREQUESTER_H
#define SERIALCIRCULARREQUESTER_H

#include <QObject>
#include <QQueue>
#include <QTimer>
#include <QElapsedTimer>
#include <QPointer>
#include "cmd/abstract_command.h"
#include "network_transport/network_transport_locker.h"
#include "network_transport/abstract_network_transport.h"
#include "uacs_network_transport/myabstractconnect.h"

/**
 * @brief Последовательно выполняет циклические и одноразовые команды устройств.
 */
class SerialCircularRequester : public QObject {
    Q_OBJECT
public:
#ifdef MYABSTRACTCONNECT_H
    explicit SerialCircularRequester(std::unique_ptr<MyAbstractConnect> transport, std::unique_ptr<NetworkTransportLocker> locker, int pollIntervalMs = 50, QObject *parent = nullptr);
    const MyAbstractConnect* getTransport();
#else
    /**
     * @brief Последовательно выполняет циклические и одноразовые команды устройств.
     *
     * @param transport Транспорт, через который отправляются команды; объект не передаётся во владение.
     * @param locker Объект управления тайм-аутами последовательного обмена.
     * @param pollIntervalMs Интервал проверки очереди команд в миллисекундах.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit SerialCircularRequester(std::unique_ptr<AbstractNetworkTransport> transport, std::unique_ptr<NetworkTransportLocker> locker, int pollIntervalMs = 50, QObject *parent = nullptr);
    /**
     * @brief Возвращает транспорт, используемый requester-ом.
     *
     * @return Текущее значение параметра.
     */
    const AbstractNetworkTransport* getTransport();
#endif
    /**
     * @brief Добавляет переиспользуемую команду в циклический опрос.
     *
     * @param cmd Команда, добавляемая в циклический опрос.
     */
void addCircularCommand(AbstractCommand *cmd);
    /**
     * @brief Добавляет команду для однократного выполнения.
     *
     * @param cmd Команда для однократного выполнения.
     */
void addDisposableCommand(AbstractCommand *cmd);
    /**
     * @brief Добавляет команду для однократного выполнения без ожидания ответа.
     *
     * @param cmd Команда для однократного выполнения.
     */
void addNoResponceCommand(AbstractCommand *cmd);
    /**
     * @brief Удаляет конкретную команду из циклического опроса.
     *
     * @param cmd Команда, удаляемая из циклического опроса.
     */
void removeCircularCommand(AbstractCommand *cmd);
    /**
     * @brief Удаляет все команды циклического опроса.
     */
void removeCommands();
    /**
     * @brief Запускает таймер последовательного выполнения команд.
     */
void startRequest();
    /**
     * @brief Останавливает запуск новых команд.
     */
void stopRequest();
signals:
    /**
     * @brief Передаёт принятые транспортом данные подписчикам.
     */
void translateData(QByteArray);
private:
    /**
     * @brief Описывает текущую фазу выполнения команды.
     */
enum class RequestState {
        Idle,
        WaitingForWrite,
        WaitingForResponse
    };

#ifdef MYABSTRACTCONNECT_H
    std::unique_ptr<MyAbstractConnect> connect_;
#else
    std::unique_ptr<AbstractNetworkTransport> transport_; /**< Хранит transport. */
#endif
    QTimer *timer; /**< Таймер запуска следующей команды. */
    std::unique_ptr<NetworkTransportLocker> locker_; /**< Хранит locker. */
    QList<AbstractCommand *> circular_commands_; /**< Команда или набор команд circular commands. */
    QQueue<AbstractCommand *> disposable_commands_; /**< Команда или набор команд disposable commands. */
    QQueue<AbstractCommand *> noresponce_commands_; /**< Команда или набор команд, не предполагающих ответа и его ожидания. */
    QPointer<AbstractCommand> current_cmd_; /**< Хранит current cmd. */
    QByteArray pending_packet_; /**< Данные pending packet. */
    QByteArray early_response_buffer_; /**< Хранит early response buffer. */
    quint64 pending_packet_id_ = 0; /**< Данные pending packet id. */
    RequestState state_ = RequestState::Idle; /**< Хранит state. */
    QElapsedTimer response_timer_; /**< Таймер response timer. */
    bool current_is_disposable_ = false; /**< Хранит current is disposable. */
    bool prefer_disposable_ = true; /**< Хранит prefer disposable. */
    bool delete_current_when_idle_ = false; /**< Хранит delete current when idle. */
    bool current_is_no_response_ = false; /**< Хранит current is no responce. */
    bool prefer_no_response_ = false; /**< Хранит prefer no responce. */
    bool repeat_current_no_response_ = false; /**< Показывает, изменилось ли значение команды, которая сформировалась, но пока она еще не отправилась. */
    int read_index_ = 0; /**< Хранит read index. */
    /**
     * @brief Отклоняет команду, которая не смогла сформировать пакет.
     */
void rejectCurrentCommand();
    /**
     * @brief Сбрасывает состояние завершённой команды.
     */
void finishCurrentCommand();
private slots:
    /**
     * @brief Выбирает следующую команду и ставит её пакет в транспорт.
     */
void processNext();
    /**
     * @brief Передаёт принятые данные активной команде и завершает её при полном ответе.
     *
     * @param data Входные данные или полезная нагрузка ответа.
     */
void unlock(QByteArray data);
    /**
     * @brief Переводит команду в ожидание ответа после полной записи пакета.
     *
     * @param packetId Уникальный идентификатор пакета в очереди транспорта.
     * @param packet Пакет, полностью переданный в буфер ввода-вывода.
     */
void onPacketAccepted(quint64 packetId, const QByteArray &packet);
};

#endif // SERIALCIRCULARREQUESTER_H
