#ifndef COMPASSLCC5000PARSER_H
#define COMPASSLCC5000PARSER_H

#include <QObject>
#include <QDataStream>
#include <QIODevice>
#include "algorithm/crc8.h"

/**
 * @brief Потоково разбирает ответы компаса LC-C5000.
 */
class CompassLCC5000Parser : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Потоково разбирает ответы компаса LC-C5000.
     *
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit CompassLCC5000Parser(QObject *parent = nullptr);
    /**
     * @brief Потоково разбирает ответы компаса LC-C5000.
     *
     * @param deviceAddr Адрес управляемого устройства.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit CompassLCC5000Parser(quint8 deviceAddr, QObject *parent = nullptr);
public slots:
    /**
     * @brief Добавляет входные данные в буфер и разбирает все полные кадры.
     *
     * @param data Входные данные или полезная нагрузка ответа.
     * @return true, если найден и обработан полный корректный кадр.
     */
bool parseReply(const QByteArray &data);

signals:
    /**
     * @brief Сообщает о разобранных данных корректного кадра.
     */
void dataReady(QByteArray, quint8);
    /**
     * @brief Передаёт последний выделенный кадр для диагностики.
     */
void lastAnswer(QByteArray);
private:
    QByteArray m_buffer; /**< Накопительный буфер входных данных. */
    quint8 m_deviceAddr = 0; /**< Адрес device addr. */
};

#endif // COMPASSLCC5000PARSER_H
