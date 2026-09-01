#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#include <QObject>

/**
 * @brief Содержит операции над байтами, словами и регистрами Modbus.
 */
class BitUtils {
public:
    /**
     * @brief Содержит операции над байтами, словами и регистрами Modbus.
     */
explicit BitUtils() {

    }

    /**
     * @brief Возвращает младший байт 16-битного значения.
     *
     * @param value Новое значение параметра.
     * @return Результат операции типа quint8.
     */
static quint8 Low(quint16 value) {
        return static_cast<quint8>(value & 0xFF);
    }
    /**
     * @brief Возвращает старший байт 16-битного значения.
     *
     * @param value Новое значение параметра.
     * @return Результат операции типа quint8.
     */
static quint8 High(quint16 value) {
        return static_cast<quint8>(value >> 8);
    }

    /**
     * @brief Собирает число с плавающей точкой из двух 16-битных слов.
     *
     * @param regs Регистры, содержащие двоичное представление числа.
     * @return Результат операции типа float.
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
     * @brief Собирает число с плавающей точкой из двух 16-битных слов.
     *
     * @param word1 Первое 16-битное слово числа.
     * @param word2 Второе 16-битное слово числа.
     * @return Результат операции типа float.
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
