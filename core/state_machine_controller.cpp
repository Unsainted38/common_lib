#include "state_machine_controller.h"
#include "event/shutdown_event.h"

StateMachineController::StateMachineController() : running_(false) {}

StateMachineController::~StateMachineController()
{
    stop();
}

void StateMachineController::start()
{
    if (running_) return;
    running_ = true;
    worker_ = std::thread(&StateMachineController::event_loop, this);
}

void StateMachineController::stop()
{
    if (!running_)
        return;
    running_ = false;

    queue_.push(std::make_shared<ShutdownEvent>());
    if (worker_.joinable())
        worker_.join();

}

void StateMachineController::post(const std::shared_ptr<IEvent> &event)
{
    queue_.push(event);
}

void StateMachineController::event_loop()
{
    while (true) {
        std::shared_ptr<IEvent> event = queue_.wait_n_pop();

        if (event->type() == EventType::Shutdown) {
            break;
        }
        machine_.handle(*event);
    }
}
