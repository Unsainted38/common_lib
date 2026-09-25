#ifndef STATE_MACHINE_CONTROLLER_H
#define STATE_MACHINE_CONTROLLER_H

#include <memory>
#include <atomic>
#include <thread>

#include "state_machine/state_machine.h"
#include "queue/priority_event_queue.h"

class StateMachineController
{
public:
    StateMachineController();
    ~StateMachineController();

    void start();
    void stop();

    void post(const std::shared_ptr<IEvent>& event);
private:
    void event_loop();

    PriorityEventQueue queue_;
    StateMachine machine_;
    std::thread worker_;
    std::atomic<bool> running_;
};

#endif // STATE_MACHINE_CONTROLLER_H
