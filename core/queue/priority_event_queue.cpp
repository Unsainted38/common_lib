#include "priority_event_queue.h"

PriorityEventQueue::PriorityEventQueue()
    : next_sequence_(0) {

}

void PriorityEventQueue::push(const std::shared_ptr<IEvent> &event)
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        QueueItem item;
        item.event = event;
        item.sequence = next_sequence_++;

        queue_.push(item);
    }

    condition_.notify_one();
}

std::shared_ptr<IEvent> PriorityEventQueue::wait_n_pop()
{
    std::unique_lock<std::mutex> lock(mutex_);

    condition_.wait(
        lock,
        [this]()
        {
            return !queue_.empty();
        });

    std::shared_ptr<IEvent> event = queue_.top().event;

    queue_.pop();

    return event;
}

bool PriorityEventQueue::empty() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}

bool PriorityEventQueue::Comparator::operator()(const QueueItem &lhs, const QueueItem &rhs) const
{
    const int lhs_priority = static_cast<int>(lhs.event->priority());
    const int rhs_priority = static_cast<int>(rhs.event->priority());
    if (lhs_priority != rhs_priority) {
        return lhs_priority < rhs_priority;
    }

    return lhs.sequence < rhs.sequence;
}
