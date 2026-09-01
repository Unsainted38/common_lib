#ifndef MSHPRPARSER_H
#define MSHPRPARSER_H

#include <QObject>
#include <QDataStream>
#include <QIODevice>
#include <QMap>
#include <QByteArray>
#include <QStringList>

/**
 * @brief Потоково разбирает текстовые ответы МШПР с проверкой контрольной суммы.
 */
class MShPRParser : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Потоково разбирает текстовые ответы МШПР с проверкой контрольной суммы.
     *
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit MShPRParser(QObject *parent = nullptr);

signals:
    /**
     * @brief Сообщает о разобранных данных корректного кадра.
     */
void dataReady(QString, QMap<QString, int>);
    /**
     * @brief Передаёт последний выделенный кадр для диагностики.
     */
void lastAnswer(QByteArray);
public slots:
    /**
     * @brief Добавляет входные данные в буфер и разбирает все полные кадры.
     *
     * @param reply Очередной фрагмент входного потока.
     * @return true, если найден и обработан полный корректный кадр.
     */
bool parseReply(const QByteArray &reply);
private:
    QByteArray m_buffer; /**< Накопительный буфер входных данных. */

};

#endif // MSHPRPARSER_H
