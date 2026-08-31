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

class SerialCircularRequester : public QObject {
    Q_OBJECT
public:
#ifdef MYABSTRACTCONNECT_H
    explicit SerialCircularRequester(MyAbstractConnect *transport, NetworkTransportLocker *locker, int pollIntervalMs = 50, QObject *parent = nullptr);
    MyAbstractConnect* getTransport();
#else
    explicit SerialCircularRequester(AbstractNetworkTransport *transport, NetworkTransportLocker *locker, int pollIntervalMs = 50, QObject *parent = nullptr);
    AbstractNetworkTransport* getTransport();
#endif
    void addCircularCommand(AbstractCommand *cmd);
    void addDisposableCommand(AbstractCommand *cmd);
    void removeCommands();
    void startRequest();
    void stopRequest();
signals:
    void translateData(QByteArray);
private:
    enum class RequestState {
        Idle,
        WaitingForWrite,
        WaitingForResponse
    };

#ifdef MYABSTRACTCONNECT_H
    MyAbstractConnect *m_connect;
#else
    AbstractNetworkTransport *m_transport;
#endif
    QTimer *timer;
    NetworkTransportLocker *m_locker;
    QList<AbstractCommand *> m_circularCommands;
    QQueue<AbstractCommand *> m_disposableCommands;
    QPointer<AbstractCommand> currentCmd;
    QByteArray m_pendingPacket;
    quint64 m_pendingPacketId = 0;
    RequestState m_state = RequestState::Idle;
    QElapsedTimer m_responseTimer;
    bool m_currentIsDisposable = false;
    bool m_preferDisposable = true;
    bool m_deleteCurrentWhenIdle = false;
    int m_readIndex = 0;
    void rejectCurrentCommand();
    void finishCurrentCommand();
private slots:
    void processNext();
    void unlock(QByteArray data);
    void onPacketAccepted(quint64 packetId, const QByteArray &packet);
};

#endif // SERIALCIRCULARREQUESTER_H
