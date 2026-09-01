#ifndef UBPCHPARSER_H
#define UBPCHPARSER_H

#include <QObject>
#include <QDataStream>
#include <QByteArray>
#include <QIODevice>

/**
 * @brief
 *
 */
class UBPChParser : public QObject {
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
explicit UBPChParser(QObject *parent = 0);
    /**
     * @brief
     *
     * @param deviceAddr
     * @param masterAddr
     * @param parent
     */
    UBPChParser(quint8 deviceAddr, quint8 masterAddr,
                QObject *parent = nullptr);
    /**
     * @brief
     *
     * @param frame
     * @return QByteArray
     */
QByteArray unpack(const QByteArray &frame);
signals:
    /**
     * @brief
     *
     * @param QByteArray
     * @param quint16
     */
void dataReady(QByteArray, quint16);
    /**
     * @brief
     *
     */
void statusOnline();
    /**
     * @brief
     *
     * @param QByteArray
     */
void lastAnswer(QByteArray);
private:
    QByteArray m_buffer; /**< TODO: describe */
    quint8 m_deviceAddr = 0; /**< TODO: describe */
    quint8 m_masterAddr = 0; /**< TODO: describe */
    bool m_filterAddresses = false; /**< TODO: describe */

public slots:
    /**
     * @brief
     *
     * @param reply
     * @return bool
     */
bool parseReply(const QByteArray &reply);
};

#endif // UBPCHPARSER_H
