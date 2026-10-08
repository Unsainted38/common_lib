#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#include <QObject>
#include <concepts>

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
    /**
     * @brief Собирает 32-битное слово из двух 16-битных слов.
     *
     * @param low_word младшее 16-битное слово числа.
     * @param high_word старшее 16-битное слово числа.
     * @return Результат операции типа quint32.
     */
static quint32 makeQuint32(quint16 lowWord, quint16 highWord) {
    return (static_cast<quint32>(highWord) << 16) | static_cast<quint32>(lowWord);
}
template <std::unsigned_integral T>
static bool bitCheck(T value, quint8 bit) {
    return (value & (T{1} << bit)) != 0;
}
template <std::unsigned_integral T>
static void setBit(T& value, quint8 bit, bool state)
{
    const T mask = T{1} << bit;

    if (state) {
        value |= mask;   // установить бит в 1
    } else {
        value &= ~mask;  // сбросить бит в 0
    }
}
};

#endif // BIT_UTILS_H
