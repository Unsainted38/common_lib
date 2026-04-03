#include "mshpr_parser.h"
#include <QDebug>

MShPRParser::MShPRParser(QObject *parent)
    : QObject(parent) {

}

bool MShPRParser::parseReply(const QByteArray &reply) {
    m_buffer.append(reply);
    int beginIndex = m_buffer.indexOf('>');
    int endIndex = m_buffer.indexOf("\r\n", beginIndex);

    if(beginIndex < 0 || endIndex < 0) {
        return false;
    }

    QByteArray packet = m_buffer.mid(beginIndex, endIndex - beginIndex + 4);
    m_buffer.remove(0, endIndex + 4);
    QByteArray body = packet.mid(0, packet.size() - 2);
    emit lastAnswer(body);
    int lastStar = body.lastIndexOf('*');

    if(lastStar < 0) {
        return false;
    }

    QByteArray payload = body.left(lastStar + 1);
    QByteArray csBytes = body.mid(lastStar + 1, 1);

    quint8 sum = 0;

    for(auto b : payload) {
        sum += static_cast<quint8>(b);
    }

    quint8 csCalc = sum & 0x7F;

    bool ok;
    quint8 csPacket = static_cast<quint8>(csBytes[0]);

    if(csPacket != csCalc) {
        qDebug() << "Checksum error";
        return false;
    }

    int slashIndex = payload.remove(0, 1).indexOf('/');

    if(slashIndex < 0) {
        return false;
    }

    QString addr = QString::fromUtf8(payload.left(slashIndex));
    QByteArray dataFields = payload.mid(slashIndex + 1);
    QStringList fieldList = QString::fromUtf8(dataFields).split('*', Qt::SkipEmptyParts);

    QMap<QString, int> fieldsMap;

    for(const QString &f : fieldList) {
        QString key;
        int value = 0;
        int eq = f.indexOf('=');

        if(eq > 0) {
            key = f.left(eq);
            value = f.mid(eq + 1).toInt(&ok);

            if(!ok) {
                value = 0;
            }
        }

        fieldsMap[key] = value;
    }

    emit dataReady(addr, fieldsMap);

    return true;
}
