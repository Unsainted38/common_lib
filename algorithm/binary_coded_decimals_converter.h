#ifndef BINARYCODEDDECIMALS_CONVERTER_H
#define BINARYCODEDDECIMALS_CONVERTER_H

#include <QtCore>
#include <QtGlobal>
class BcdConverter {
public:
    static double toDouble_SXXY(const QByteArray &bcd);
    static float toFloat_SXXY(const QByteArray &bcd);
    static double toDouble_SXXXYY(const QByteArray &bcd);
    static float toFloat_SXXXYY(const QByteArray &bcd);
    static QByteArray fromQuint8_XX(quint8 value);
    static QByteArray fromFloat_SXXY(float value);
    static QByteArray fromDouble_SXXY(double value);

};
#endif // BINARYCODEDDECIMALS_CONVERTER_H
