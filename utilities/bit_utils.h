#ifndef BITUTILS_H
#define BITUTILS_H

#include <QObject>

class BitUtils {
public:
    explicit BitUtils() {

    }

    static quint8 Low(quint16 value) {
        return static_cast<quint8>(value & 0xFF);
    }
    static quint8 High(quint16 value) {
        return static_cast<quint8>(value >> 8);
    }

    static float makeFloat(const quint16 *regs) {
        union {
            quint16 data[2];
            float value;
        } cvt;
        cvt.data[0] = regs[0];
        cvt.data[1] = regs[1];
        return cvt.value;
    }
    static float makeFloat(quint16 word1, quint16 word2) {
        union {
            quint16 data[2];
            float value;
        } cvt;
        cvt.data[0] = word1;
        cvt.data[1] = word2;
        return cvt.value;
    }
};

#endif // BITUTILS_H
