#ifndef EVENT_TYPE_H
#define EVENT_TYPE_H

enum class EventType {
    PowerOn,
    Shutdown,
    InitCompleted,
    StartExtremeTracking,
    StartAutoTracking,
    StartTargetDesignationTracking,
    StartMoving,
    StartMovingToPoint,
    Stop,
    Reset,
    Failure,
    EmergencyStop
};

#endif // EVENT_TYPE_H
