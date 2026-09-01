#ifndef MSHPRPARSER_H
#define MSHPRPARSER_H

#include <QObject>
#include <QDataStream>
#include <QIODevice>
#include <QMap>
#include <QByteArray>
#include <QStringList>

/**
 * @brief
 *
 */
class MShPRParser : public QObject {
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
explicit MShPRParser(QObject *parent = nullptr);

signals:
    /**
     * @brief
     *
     * @param QString
     * @param QMap<QString
     * @param int>
     */
void dataReady(QString, QMap<QString, int>);
    /**
     * @brief
     *
     * @param QByteArray
     */
void lastAnswer(QByteArray);
public slots:
    /**
     * @brief
     *
     * @param reply
     * @return bool
     */
bool parseReply(const QByteArray &reply);
private:
    QByteArray m_buffer; /**< TODO: describe */

};

#endif // MSHPRPARSER_H
