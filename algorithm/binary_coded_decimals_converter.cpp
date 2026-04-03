#include "binary_coded_decimals_converter.h"

double BcdConverter::toDouble_SXXY(const QByteArray &bcd) {
    if(bcd.size() < 2) {
        return 0.0;
    }

    quint8 b1 = bcd[0];
    quint8 b2 = bcd[1];

    int sign = (b1 >> 4) & 0x0F;
    int d1 = b1 & 0x0F;
    int d2 = (b2 >> 4) & 0x0F;
    int d3 = b2 & 0x0F;

    double value = d1 * 10 + d2 + d3 / 10.0;

    if(sign == 1) {
        value = -value;
    }

    return value;
}

float BcdConverter::toFloat_SXXY(const QByteArray &bcd) {
    if(bcd.size() < 2) {
        return 0.0;
    }

    quint8 b1 = bcd[0];
    quint8 b2 = bcd[1];

    int sign = (b1 >> 4) & 0x0F;
    int d1 = b1 & 0x0F;
    int d2 = (b2 >> 4) & 0x0F;
    int d3 = b2 & 0x0F;

    float value = d1 * 10 + d2 + d3 / 10.0;

    if(sign == 1) {
        value = -value;
    }

    return value;
}

double BcdConverter::toDouble_SXXXYY(const QByteArray &bcd) {
    if(bcd.size() < 3) {
        return 0.0;
    }

    quint8 b1 = bcd[0];
    quint8 b2 = bcd[1];
    quint8 b3 = bcd[2];
    int sign = (b1 >> 4) & 0x0F;
    int d1 = b1 & 0x0F;
    int d2 = (b2 >> 4) & 0x0F;
    int d3 = b2 & 0x0F;
    int d4 = (b3 >> 4) & 0x0F;
    int d5 = b3 & 0x0F;
    double value = d1 * 100 + d2 * 10 + d3 + d4 / 10.0 + d5 / 100.0;

    if(sign == 1) {
        value = -value;
    }

    return value;
}

float BcdConverter::toFloat_SXXXYY(const QByteArray &bcd) {
    if(bcd.size() < 3) {
        return 0.0;
    }

    quint8 b1 = bcd[0];
    quint8 b2 = bcd[1];
    quint8 b3 = bcd[2];
    int sign = (b1 >> 4) & 0x0F;
    int d1 = b1 & 0x0F;
    int d2 = (b2 >> 4) & 0x0F;
    int d3 = b2 & 0x0F;
    int d4 = (b3 >> 4) & 0x0F;
    int d5 = b3 & 0x0F;
    float value = d1 * 100 + d2 * 10 + d3 + d4 / 10.0 + d5 / 100.0;

    if(sign == 1) {
        value = -value;
    }

    return value;
}

QByteArray BcdConverter::fromQuint8_XX(quint8 value) {
    QByteArray bcd;
    int d1 = (value / 10) % 10;
    int d2 = value % 10;

    quint8 b1 = (d1 << 4) | d2;
    bcd.append(b1);

    return bcd;
}

QByteArray BcdConverter::fromFloat_SXXY(float value) {
    QByteArray bcd;
    bool negative = value < 0;

    if(negative) {
        value = -value;
    }

    int scaled = qRound(value * 10);
    int d1 = (scaled / 100) % 10;
    int d2 = (scaled / 10) % 10;
    int d3 = scaled % 10;

    quint8 b1 = ((negative ? 1 : 0) << 4) | d1;
    quint8 b2 = (d2 << 4) | d3;
    bcd.append(b1);
    bcd.append(b2);
    return bcd;
}

QByteArray BcdConverter::fromDouble_SXXY(double value) {
    QByteArray bcd;
    bool negative = value < 0;

    if(negative) {
        value = -value;
    }

    int scaled = qRound(value * 10);
    int d1 = (scaled / 100) % 10;
    int d2 = (scaled / 10) % 10;
    int d3 = scaled % 10;

    quint8 b1 = ((negative ? 1 : 0) << 4) | d1;
    quint8 b2 = (d2 << 4) | d3;
    bcd.append(b1);
    bcd.append(b2);
    return bcd;
}
