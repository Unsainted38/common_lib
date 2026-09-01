#ifndef COMPASSLCC5000PARSER_H
#define COMPASSLCC5000PARSER_H

#include <QObject>
#include <QDataStream>
#include <QIODevice>
#include "algorithm/crc8.h"

/**
 * @brief
 *
 */
class CompassLCC5000Parser : public QObject {
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
explicit CompassLCC5000Parser(QObject *parent = nullptr);
    /**
     * @brief
     *
     * @param deviceAddr
     * @param parent
     */
explicit CompassLCC5000Parser(quint8 deviceAddr, QObject *parent = nullptr);
public slots:
    /**
     * @brief
     *
     * @param data
     * @return bool
     */
bool parseReply(const QByteArray &data);

signals:
    /**
     * @brief
     *
     * @param QByteArray
     * @param quint8
     */
void dataReady(QByteArray, quint8);
    /**
     * @brief
     *
     * @param QByteArray
     */
void lastAnswer(QByteArray);
private:
    QByteArray m_buffer; /**< TODO: describe */
    quint8 m_deviceAddr = 0; /**< TODO: describe */
};

#endif // COMPASSLCC5000PARSER_H
