#ifndef VESPER_DRIVER_H
#define VESPER_DRIVER_H

#include "i_driver.h"

class VesperDriver : public IDriver
{
public:
    VesperDriver();

    // IDriver interface
public:
    void move(float speed, MoveDirection direction) const override;
    void stop() const override;
};

#endif // VESPER_DRIVER_H
