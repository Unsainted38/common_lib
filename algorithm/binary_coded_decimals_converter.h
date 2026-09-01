#ifndef BINARYCODEDDECIMALS_CONVERTER_H
#define BINARYCODEDDECIMALS_CONVERTER_H

#include <QtCore>
#include <QtGlobal>
/**
 * @brief
 *
 */
class BcdConverter {
public:
    /**
     * @brief
     *
     * @param bcd
     * @return double
     */
static double toDouble_SXXY(const QByteArray &bcd);
    /**
     * @brief
     *
     * @param bcd
     * @return float
     */
static float toFloat_SXXY(const QByteArray &bcd);
    /**
     * @brief
     *
     * @param bcd
     * @return double
     */
static double toDouble_SXXXYY(const QByteArray &bcd);
    /**
     * @brief
     *
     * @param bcd
     * @return float
     */
static float toFloat_SXXXYY(const QByteArray &bcd);
    /**
     * @brief
     *
     * @param value
     * @return QByteArray
     */
static QByteArray fromQuint8_XX(quint8 value);
    /**
     * @brief
     *
     * @param value
     * @return QByteArray
     */
static QByteArray fromFloat_SXXY(float value);
    /**
     * @brief
     *
     * @param value
     * @return QByteArray
     */
static QByteArray fromDouble_SXXY(double value);

};
#endif // BINARYCODEDDECIMALS_CONVERTER_H
