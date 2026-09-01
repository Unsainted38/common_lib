#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#include <QObject>

/**
 * @brief
 *
 */
class BitUtils {
public:
    /**
     * @brief
     *
     */
explicit BitUtils() {

    }

    /**
     * @brief
     *
     * @param value
     * @return quint8
     */
static quint8 Low(quint16 value) {
        return static_cast<quint8>(value & 0xFF);
    }
    /**
     * @brief
     *
     * @param value
     * @return quint8
     */
static quint8 High(quint16 value) {
        return static_cast<quint8>(value >> 8);
    }

    /**
     * @brief
     *
     * @param regs
     * @return float
     */
static float makeFloat(const quint16 *regs) {
        union {
            quint16 data[2];
            float value;
        } cvt;
        cvt.data[0] = regs[0];
        cvt.data[1] = regs[1];
        return cvt.value;
    }
    /**
     * @brief
     *
     * @param word1
     * @param word2
     * @return float
     */
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

#endif // BIT_UTILS_H
