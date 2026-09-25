#ifndef STATE_ID_H
#define STATE_ID_H

enum class StateId {
    Initializing,
    Operational,
    Idle,
    MovingToPoint,
    Moving,
    ExtremeTracking,
    AutoTracking,
    TargetDesignationTracking,
    Error
};

#endif // STATE_ID_H
