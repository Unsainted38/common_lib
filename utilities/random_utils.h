#ifndef RANDOM_UTILS_H
#define RANDOM_UTILS_H
#include <QRandomGenerator>

/**
 * @brief Генерирует псевдослучайное вещественное число в заданном диапазоне.
 *
 * @param min Нижняя граница диапазона.
 * @param max Верхняя граница диапазона.
 * @return Результат операции типа double.
 */
double randomDouble(double min, double max)
{
    return min + QRandomGenerator::global()->generateDouble() * (max - min);
}

#endif // RANDOM_UTILS_H
