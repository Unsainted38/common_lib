#ifndef SERIALCIRCULARREQUESTER_H
#define SERIALCIRCULARREQUESTER_H

#include <QObject>
#include <QQueue>
#include <QTimer>
#include <mach/mach.h>
#include <mach/task.h>
#include "cmd/abstract_command.h"
#include "network_transport/network_transport_locker.h"
#include "network_transport/abstract_network_transport.h"
//#include "uacs_network_transport/myabstractconnect.h"

class SerialCircularRequester : public QObject {
    Q_OBJECT
public:
#ifdef MYABSTRACTCONNECT_H
    explicit SerialCircularRequester(MyAbstractConnect *transport, NetworkTransportLocker *locker, int pollIntervalMs = 50, QObject *parent = nullptr);
#else
    explicit SerialCircularRequester(AbstractNetworkTransport *transport, NetworkTransportLocker *locker, int pollIntervalMs = 50, QObject *parent = nullptr);
#endif
    void addCircularCommand(AbstractCommand *cmd);
    void addDisposableCommand(AbstractCommand *cmd);
    void removeCommands();
    void startRequest();
signals:
    void translateData(QByteArray);
private:
    size_t currentMemoryUsage();
#ifdef MYABSTRACTCONNECT_H
    MyAbstractConnect *m_connect;
#else
    AbstractNetworkTransport *m_transport;
#endif
    QTimer *timer;
    NetworkTransportLocker *m_locker;
    QList<AbstractCommand *> m_circularCommands;
    QQueue<AbstractCommand *> m_disposableCommands;
    AbstractCommand *currentCmd;
    int m_readIndex = 0;
private slots:
    void processNext();
    void unlock();
};

#endif // SERIALCIRCULARREQUESTER_H
