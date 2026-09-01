#ifndef BINARYCODEDDECIMALS_CONVERTER_H
#define BINARYCODEDDECIMALS_CONVERTER_H

#include <QtCore>
#include <QtGlobal>
/**
 * @brief Преобразует двоично-десятичные представления значений компаса.
 */
class BcdConverter {
public:
    /**
     * @brief Преобразует входное значение в double sxxy.
     *
     * @param bcd Байты двоично-десятичного представления.
     * @return Результат операции типа double.
     */
static double toDouble_SXXY(const QByteArray &bcd);
    /**
     * @brief Преобразует входное значение в float sxxy.
     *
     * @param bcd Байты двоично-десятичного представления.
     * @return Результат операции типа float.
     */
static float toFloat_SXXY(const QByteArray &bcd);
    /**
     * @brief Преобразует входное значение в double sxxxyy.
     *
     * @param bcd Байты двоично-десятичного представления.
     * @return Результат операции типа double.
     */
static double toDouble_SXXXYY(const QByteArray &bcd);
    /**
     * @brief Преобразует входное значение в float sxxxyy.
     *
     * @param bcd Байты двоично-десятичного представления.
     * @return Результат операции типа float.
     */
static float toFloat_SXXXYY(const QByteArray &bcd);
    /**
     * @brief Кодирует значение в формат quint8 xx.
     *
     * @param value Новое значение параметра.
     * @return Результат операции типа QByteArray.
     */
static QByteArray fromQuint8_XX(quint8 value);
    /**
     * @brief Кодирует значение в формат float sxxy.
     *
     * @param value Новое значение параметра.
     * @return Результат операции типа QByteArray.
     */
static QByteArray fromFloat_SXXY(float value);
    /**
     * @brief Кодирует значение в формат double sxxy.
     *
     * @param value Новое значение параметра.
     * @return Результат операции типа QByteArray.
     */
static QByteArray fromDouble_SXXY(double value);

};
#endif // BINARYCODEDDECIMALS_CONVERTER_H
