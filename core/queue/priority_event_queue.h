#ifndef PRIORITY_EVENT_QUEUE_H
#define PRIORITY_EVENT_QUEUE_H

#include <condition_variable>
#include <cstdint>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>

#include <core/event/i_event.h>


class PriorityEventQueue
{
public:
    PriorityEventQueue();

    void push(const std::shared_ptr<IEvent>& event);

    std::shared_ptr<IEvent> wait_n_pop();

    bool empty() const;
private:
    struct QueueItem {
        std::shared_ptr<IEvent> event;
        std::uint64_t sequence;
    };
    struct Comparator {
        bool operator()(const QueueItem& lhs,
                        const QueueItem& rhs) const;
    };

    std::priority_queue<
        QueueItem,
        std::vector<QueueItem>,
        Comparator
        > queue_;
    mutable std::mutex mutex_;
    std::condition_variable condition_;

    std::uint64_t next_sequence_;
};

#endif // PRIORITY_EVENT_QUEUE_H
