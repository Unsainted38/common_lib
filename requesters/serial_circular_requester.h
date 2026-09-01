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
 * @brief
 *
 */
class SerialCircularRequester : public QObject {
    Q_OBJECT
public:
#ifdef MYABSTRACTCONNECT_H
    explicit SerialCircularRequester(MyAbstractConnect *transport, NetworkTransportLocker *locker, int pollIntervalMs = 50, QObject *parent = nullptr);
    MyAbstractConnect* getTransport();
#else
    /**
     * @brief
     *
     * @param transport
     * @param locker
     * @param pollIntervalMs
     * @param parent
     */
explicit SerialCircularRequester(AbstractNetworkTransport *transport, NetworkTransportLocker *locker, int pollIntervalMs = 50, QObject *parent = nullptr);
    /**
     * @brief
     *
     * @return AbstractNetworkTransport
     */
AbstractNetworkTransport* getTransport();
#endif
    /**
     * @brief
     *
     * @param cmd
     */
void addCircularCommand(AbstractCommand *cmd);
    /**
     * @brief
     *
     * @param cmd
     */
void addDisposableCommand(AbstractCommand *cmd);
    /**
     * @brief
     *
     * @param cmd
     */
void removeCircularCommand(AbstractCommand *cmd);
    /**
     * @brief
     *
     */
void removeCommands();
    /**
     * @brief
     *
     */
void startRequest();
    /**
     * @brief
     *
     */
void stopRequest();
signals:
    /**
     * @brief
     *
     * @param QByteArray
     */
void translateData(QByteArray);
private:
    /**
     * @brief
     *
     */
enum class RequestState {
        Idle,
        WaitingForWrite,
        WaitingForResponse
    };

#ifdef MYABSTRACTCONNECT_H
    MyAbstractConnect *m_connect;
#else
    AbstractNetworkTransport *m_transport; /**< TODO: describe */
#endif
    QTimer *timer; /**< TODO: describe */
    NetworkTransportLocker *m_locker; /**< TODO: describe */
    QList<AbstractCommand *> m_circularCommands; /**< TODO: describe */
    QQueue<AbstractCommand *> m_disposableCommands; /**< TODO: describe */
    QPointer<AbstractCommand> currentCmd; /**< TODO: describe */
    QByteArray m_pendingPacket; /**< TODO: describe */
    QByteArray m_earlyResponseBuffer; /**< TODO: describe */
    quint64 m_pendingPacketId = 0; /**< TODO: describe */
    RequestState m_state = RequestState::Idle; /**< TODO: describe */
    QElapsedTimer m_responseTimer; /**< TODO: describe */
    bool m_currentIsDisposable = false; /**< TODO: describe */
    bool m_preferDisposable = true; /**< TODO: describe */
    bool m_deleteCurrentWhenIdle = false; /**< TODO: describe */
    int m_readIndex = 0; /**< TODO: describe */
    /**
     * @brief
     *
     */
void rejectCurrentCommand();
    /**
     * @brief
     *
     */
void finishCurrentCommand();
private slots:
    /**
     * @brief
     *
     */
void processNext();
    /**
     * @brief
     *
     * @param data
     */
void unlock(QByteArray data);
    /**
     * @brief
     *
     * @param packetId
     * @param packet
     */
void onPacketAccepted(quint64 packetId, const QByteArray &packet);
};

#endif // SERIALCIRCULARREQUESTER_H
