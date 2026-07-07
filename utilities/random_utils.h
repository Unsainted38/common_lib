#ifndef RANDOM_UTILS_H
#define RANDOM_UTILS_H
#include <QRandomGenerator>

double randomDouble(double min, double max)
{
    return min + QRandomGenerator::global()->generateDouble() * (max - min);
}

#endif // RANDOM_UTILS_H
