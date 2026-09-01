#ifndef UBPCHPARSER_H
#define UBPCHPARSER_H

#include <QObject>
#include <QDataStream>
#include <QByteArray>
#include <QIODevice>

/**
 * @brief Потоково разбирает экранированные кадры УБПЧ с проверкой CRC16.
 */
class UBPChParser : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Потоково разбирает экранированные кадры УБПЧ с проверкой CRC16.
     *
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit UBPChParser(QObject *parent = 0);
    /**
     * @brief Потоково разбирает экранированные кадры УБПЧ с проверкой CRC16.
     *
     * @param deviceAddr Адрес управляемого устройства.
     * @param masterAddr Адрес ведущего устройства.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
    UBPChParser(quint8 deviceAddr, quint8 masterAddr,
                QObject *parent = nullptr);
    /**
     * @brief Удаляет байты экранирования из принятого кадра.
     *
     * @param frame Структура, в которую записывается разобранный кадр.
     * @return Сформированный массив байтов.
     */
QByteArray unpack(const QByteArray &frame);
signals:
    /**
     * @brief Сообщает о разобранных данных корректного кадра.
     */
void dataReady(QByteArray, quint16);
    /**
     * @brief Сообщает о получении корректного ответа устройства.
     */
void statusOnline();
    /**
     * @brief Передаёт последний выделенный кадр для диагностики.
     */
void lastAnswer(QByteArray);
private:
    QByteArray m_buffer; /**< Накопительный буфер входных данных. */
    quint8 m_deviceAddr = 0; /**< Адрес device addr. */
    quint8 m_masterAddr = 0; /**< Адрес master addr. */
    bool m_filterAddresses = false; /**< Адрес filter addresses. */

public slots:
    /**
     * @brief Добавляет входные данные в буфер и разбирает все полные кадры.
     *
     * @param reply Очередной фрагмент входного потока.
     * @return true, если найден и обработан полный корректный кадр.
     */
bool parseReply(const QByteArray &reply);
};

#endif // UBPCHPARSER_H
