#include "mshpr_parser.h"
#include <QDebug>

MShPRParser::MShPRParser(QObject *parent)
    : QObject(parent) {

}

bool MShPRParser::parseReply(const QByteArray &reply) {
    m_buffer.append(reply);
    bool parsedAny = false;

    while (true) {
        const qsizetype beginIndex = m_buffer.indexOf('>');
        if (beginIndex < 0) {
            m_buffer.clear();
            return parsedAny;
        }

        if (beginIndex > 0) {
            m_buffer.remove(0, beginIndex);
        }

        const qsizetype endIndex = m_buffer.indexOf("\r\n");
        if (endIndex < 0) {
            return parsedAny;
        }

        const QByteArray body = m_buffer.left(endIndex);
        m_buffer.remove(0, endIndex + 2);
        emit lastAnswer(body);

        const qsizetype lastStar = body.lastIndexOf('*');
        if (lastStar < 0 || lastStar + 1 >= body.size()) {
            continue;
        }

        QByteArray payload = body.first(lastStar + 1);
        quint8 sum = 0;
        for (const char byte : std::as_const(payload)) {
            sum += static_cast<quint8>(byte);
        }

        const quint8 csCalc = sum & 0x7F;
        const quint8 csPacket =
            static_cast<quint8>(body.at(lastStar + 1));
        if (csPacket != csCalc) {
            qDebug() << "Checksum error";
            continue;
        }

        payload.remove(0, 1); // Символ '>'.
        const qsizetype slashIndex = payload.indexOf('/');
        if (slashIndex < 0) {
            continue;
        }

        const QString addr = QString::fromUtf8(payload.first(slashIndex));
        const QStringList fieldList =
            QString::fromUtf8(payload.sliced(slashIndex + 1))
                .split('*', Qt::SkipEmptyParts);

        QMap<QString, int> fieldsMap;
        for (const QString &field : fieldList) {
            const qsizetype equalsIndex = field.indexOf('=');
            if (equalsIndex <= 0) {
                continue;
            }

            bool ok = false;
            const int value = field.sliced(equalsIndex + 1).toInt(&ok);
            if (ok) {
                fieldsMap.insert(field.first(equalsIndex), value);
            }
        }

        emit dataReady(addr, fieldsMap);
        parsedAny = true;
    }
}
