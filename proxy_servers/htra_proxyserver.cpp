#include "htra_proxyserver.h"

HtraProxyServer::HtraProxyServer(QString configPath, QString section, QObject *parent)
    : QObject(parent),
      configPath(configPath),
      section(section) {
    loadConfig();
    server = new QTcpServer(this);

    if(server->listen(QHostAddress::Any, port)) {
        qDebug() << name << "started on ip" << ServerAddress << ":" << port << Qt::endl;
    } else {
        qDebug() << name << "start failed on ip" << ServerAddress << ":" << port << Qt::endl;
    }

    processor = new THrttaProcessor(this);
    connect(server, &QTcpServer::newConnection, [this]() {
        currentClient = server->nextPendingConnection();
        connect(currentClient, &QTcpSocket::readyRead, this, &HtraProxyServer::onParseRawData);
    });
}

void HtraProxyServer::loadConfig() {
    QSettings settings(configPath, QSettings::IniFormat);
    settings.beginGroup(section);
    ServerAddress = QHostAddress(settings.value("HostIp", QHostAddress::LocalHost).toString());
    ListenIp = QHostAddress(settings.value("ListenIp", QHostAddress::Any).toString());
    port = settings.value("Port", 7777).toUInt();
    name = settings.value("Name", "tcp_proxy").toString();
    settings.endGroup();
}

void HtraProxyServer::onParseRawData() {
    buffer.append(currentClient->readAll());
    QByteArray header;
    QDataStream out(header);
    out.setByteOrder(QDataStream::LittleEndian);
    out << HEADER;
    int headerIndex = buffer.indexOf(header);

    if(headerIndex < 0) {
        return;
    }

    buffer.remove(0, headerIndex);

    if(buffer.size() < 3) {
        return;
    }

    quint8 packetLength = buffer[2];

    if(buffer.size() < packetLength + 2) {
        return;
    }

    QByteArray packet = buffer.mid(headerIndex + 2, packetLength);
    buffer.remove(buffer.indexOf(packet), packet.size());
    onGetPacket(packet);
}

void HtraProxyServer::onGetPacket(QByteArray packet) {
    QDataStream in(&packet, QIODevice::ReadOnly);
    in.setByteOrder(QDataStream::LittleEndian);
    quint8 length;
    quint8 cmdId;
    quint8 cmdType;
    quint64 value;
    in >> length
       >> cmdType
       >> cmdId;

    if(cmdType == READ_CMD_TYPE) {
        sendResponse(cmdId);
    } else if(cmdType == WRITE_CMD_TYPE) {
        in >> value;

        switch(cmdId) {
            case HTRA_CMD::CENTER_FREQ:
                processor->onSetCenter(value);
                break;

            case HTRA_CMD::LEVEL:
                processor->onSetLevel(value);
                break;

            case HTRA_CMD::PICK:

                break;

            case HTRA_CMD::PICK_SEARCH_CENTER:
                processor->onSetPickSearchCenter(value);
                break;

            case HTRA_CMD::PICK_SEARCH_FULL_SPAN:
                processor->onSetPickSearchFullSpan();
                break;

            case HTRA_CMD::PICK_SEARCH_TYPE:
                processor->onSetPickSearchType(value);
                break;

            case HTRA_CMD::PICK_SEARCH_WIDTH:
                processor->onSetPickSearchWidth(value);
                break;

            case HTRA_CMD::RBW:
                processor->onSetRbw(value);
                break;

            case HTRA_CMD::SPAN:
                processor->onSetSpan(value);
                break;

            case HTRA_CMD::VBW:
                processor->onSetVbw(value);
                break;

        }
    }
}

void HtraProxyServer::sendResponse(quint8 cmdId) {
    QByteArray reply;
    QDataStream out(&reply, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);
    out << READ_CMD_TYPE;
    out << cmdId;

    switch(cmdId) {
        case HTRA_CMD::CENTER_FREQ:
            out << static_cast<quint64>(processor->centerFreq());
            break;

        case HTRA_CMD::LEVEL:
            out << static_cast<quint64>(processor->level());
            break;

        case HTRA_CMD::PICK:
            out << static_cast<quint64>(processor->pick());
            break;

        case HTRA_CMD::PICK_SEARCH_CENTER:
            out << static_cast<quint64>(processor->pickSearchCenter());
            break;

        case HTRA_CMD::PICK_SEARCH_FULL_SPAN:

            break;

        case HTRA_CMD::PICK_SEARCH_TYPE:
            out << static_cast<quint64>(processor->pickSearchType());
            break;

        case HTRA_CMD::PICK_SEARCH_WIDTH:
            out << static_cast<quint64>(processor->pickSearchWidth());
            break;

        case HTRA_CMD::RBW:
            out << static_cast<quint64>(processor->rbw());
            break;

        case HTRA_CMD::SPAN:
            out << static_cast<quint64>(processor->span());
            break;

        case HTRA_CMD::VBW:
            out << static_cast<quint64>(processor->vbw());
            break;

    }

    reply.push_front(reply.size());
    reply.push_front(0xAA);
    reply.push_front(0x55);
    currentClient->write(reply);
}
