#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <chrono>

template<typename T>
struct Measurement {
    T value{};
    std::chrono::steady_clock::time_point timestamp{};
    bool valid = false;
};


template<typename T>
bool isFresh(
    const Measurement<T>& measurement,
    std::chrono::steady_clock::time_point now,
    std::chrono::steady_clock::duration maxAge)
{
    return measurement.valid
           && measurement.timestamp <= now
           && now - measurement.timestamp <= maxAge;
}


#endif // MEASUREMENT_H
