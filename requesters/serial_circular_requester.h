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
//#include "uacs_network_transport/myabstractconnect.h"

/**
 * @brief Последовательно выполняет циклические и одноразовые команды устройств.
 */
class SerialCircularRequester : public QObject {
    Q_OBJECT
public:
#ifdef MYABSTRACTCONNECT_H
    explicit SerialCircularRequester(MyAbstractConnect *transport, NetworkTransportLocker *locker, int pollIntervalMs = 50, QObject *parent = nullptr);
    MyAbstractConnect* getTransport();
#else
    /**
     * @brief Последовательно выполняет циклические и одноразовые команды устройств.
     *
     * @param transport Транспорт, через который отправляются команды; объект не передаётся во владение.
     * @param locker Объект управления тайм-аутами последовательного обмена.
     * @param pollIntervalMs Интервал проверки очереди команд в миллисекундах.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit SerialCircularRequester(AbstractNetworkTransport *transport, NetworkTransportLocker *locker, int pollIntervalMs = 50, QObject *parent = nullptr);
    /**
     * @brief Возвращает транспорт, используемый requester-ом.
     *
     * @return Текущее значение параметра.
     */
AbstractNetworkTransport* getTransport();
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
    MyAbstractConnect *m_connect;
#else
    AbstractNetworkTransport *m_transport; /**< Хранит transport. */
#endif
    QTimer *timer; /**< Таймер запуска следующей команды. */
    NetworkTransportLocker *m_locker; /**< Хранит locker. */
    QList<AbstractCommand *> m_circularCommands; /**< Команда или набор команд circular commands. */
    QQueue<AbstractCommand *> m_disposableCommands; /**< Команда или набор команд disposable commands. */
    QPointer<AbstractCommand> currentCmd; /**< Хранит current cmd. */
    QByteArray m_pendingPacket; /**< Данные pending packet. */
    QByteArray m_earlyResponseBuffer; /**< Хранит early response buffer. */
    quint64 m_pendingPacketId = 0; /**< Данные pending packet id. */
    RequestState m_state = RequestState::Idle; /**< Хранит state. */
    QElapsedTimer m_responseTimer; /**< Таймер response timer. */
    bool m_currentIsDisposable = false; /**< Хранит current is disposable. */
    bool m_preferDisposable = true; /**< Хранит prefer disposable. */
    bool m_deleteCurrentWhenIdle = false; /**< Хранит delete current when idle. */
    int m_readIndex = 0; /**< Хранит read index. */
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
