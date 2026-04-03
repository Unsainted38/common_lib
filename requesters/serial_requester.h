#ifndef SERIALREQUESTER_H
#define SERIALREQUESTER_H

#include <QQueue>
#include <QTimer>
#include <functional>
#include "parsers/ubpch_parser.h"
#include "network_transport/network_transport_locker.h"
#include "network_transport/abstract_network_transport.h"



class SerialRequester : public QObject {
    struct UBPChRequest {
        QByteArray command;
        std::function<void(const QByteArray &)> onReply;
        int timeoutMs;
    };

    Q_OBJECT
public:
    explicit SerialRequester(AbstractNetworkTransport *transport, NetworkTransportLocker locker, QObject *parent = nullptr);

    void enqueue(const QByteArray &command,
                 std::function<void(const QByteArray &)> onReply,
                 int timeoutMs = 40);
    void enqueueConcurrent(const QByteArray &command,
                           std::function<void(const QByteArray &)> onReply,
                           int timeoutMs = 40);
    void startRequest();

private slots:
    void onDataReceived(const QByteArray &data);
    void onTimeout();
    void onParsedData(const QByteArray &data);
signals:
    void translateData(QByteArray);
private:
    AbstractNetworkTransport *m_transport;
    UBPChParser parser;
    QQueue<UBPChRequest> queue;
    QTimer timeoutTimer;
    NetworkTransportLocker m_locker;
    bool busy = false;
    UBPChRequest current;
    bool m_concurrentFlag = false;
    int m_currentIndex;

    void processNext();

};
#endif // SERIALREQUESTER_H
