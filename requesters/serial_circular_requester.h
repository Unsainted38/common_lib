#ifndef SERIALCIRCULARREQUESTER_H
#define SERIALCIRCULARREQUESTER_H

#include <QObject>
#include <QQueue>
#include <QTimer>
#include <mach/mach.h>
#include <mach/task.h>
#include "parsers/ubpch_parser.h"
#include "cmd/abstract_command.h"
#include "network_transport/network_transport_locker.h"
#include "uacs_network_transport/myabstractconnect.h"

class SerialCircularRequester : public QObject {
    Q_OBJECT
public:
#ifdef MYABSTRACTCONNECT_H
    explicit SerialCircularRequester(MyAbstractConnect *transport, NetworkTransportLocker *locker, int pollIntervalMs = 50, QObject *parent = nullptr);
#else
    explicit SerialCircularRequester(AbstractNetworkTransport *transport, NetworkTransportLocker *locker, int pollIntervalMs = 50, QObject *parent = nullptr);
#endif
    void addCommand(AbstractCommand *cmd);
    void addExtraCommand(AbstractCommand *cmd);
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
    QTimer *memTimer;
    NetworkTransportLocker *m_locker;
    QList<AbstractCommand *> m_readCommands;
    QQueue<AbstractCommand *> m_writeCommands;
    AbstractCommand *currentCmd;
    int m_readIndex = 0;
private slots:
    void processNext();
    void unlock();
};

#endif // SERIALCIRCULARREQUESTER_H
