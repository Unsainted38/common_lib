#ifndef HTRAPROXYSERVER_H
#define HTRAPROXYSERVER_H

#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>
#include <QSettings>
#include <QDataStream>
#include <QIODevice>
#include "processors/hrtta_processor.h"

const quint16 HEADER = 0x55AA;
const quint8 READ_CMD_TYPE = 0x04;
const quint8 WRITE_CMD_TYPE = 0x05;

struct HTRA_CMD {
    inline static const quint8 CENTER_FREQ = 0x01;
    inline static const quint8 LEVEL = 0x02;
    inline static const quint8 SPAN = 0x03;
    inline static const quint8 RBW = 0x04;
    inline static const quint8 VBW = 0x05;
    inline static const quint8 PICK = 0x06;
    inline static const quint8 PICK_SEARCH_TYPE = 0x07;
    inline static const quint8 PICK_SEARCH_CENTER = 0x08;
    inline static const quint8 PICK_SEARCH_WIDTH = 0x09;
    inline static const quint8 PICK_SEARCH_FULL_SPAN = 0x10;
};

class HtraProxyServer : public QObject {
    Q_OBJECT
    QString configPath;
    QString section;
    QTcpServer *server;
    QHostAddress ServerAddress;
    QHostAddress ListenIp;
    quint16 port;
    QString name;
    THrttaProcessor *processor;
    QByteArray buffer;
    QTcpSocket *currentClient;
public:
    explicit HtraProxyServer(QString configPath, QString section, QObject *parent = nullptr);

signals:
    void translateData(QByteArray);
private:
    void loadConfig();
    void onParseRawData();
    void onGetPacket(QByteArray packet);
    void sendResponse(quint8 cmdId);
};

#endif // HTRAPROXYSERVER_H
