#include "serial_circular_requester.h"
#ifdef MYABSTRACTCONNECT_H
SerialCircularRequester::SerialCircularRequester(std::unique_ptr<MyAbstractConnect> transport, std::unique_ptr<NetworkTransportLocker> locker, int pollIntervalMs, QObject *parent)
    : QObject(parent),
    connect_(std::move(transport)),
      timer(new QTimer(this)),
      locker_(std::move(locker)) {
    timer->setInterval(pollIntervalMs);
    connect(timer, &QTimer::timeout, this, &SerialCircularRequester::processNext);
    connect(connect_.get(), &MyAbstractConnect::readyToProcessData, this, &SerialCircularRequester::translateData);
    connect(connect_.get(), SIGNAL(readyToProcessData(QByteArray)), this, SLOT(unlock(QByteArray)), Qt::UniqueConnection);
}

const MyAbstractConnect* SerialCircularRequester::getTransport()
{
    return connect_.get();
}
#else
SerialCircularRequester::SerialCircularRequester(std::unique_ptr<AbstractNetworkTransport> transport, std::unique_ptr<NetworkTransportLocker> locker, int pollIntervalMs, QObject *parent)
    : QObject(parent),
      transport_(std::move(transport)),
      timer(new QTimer(this)),
      locker_(std::move(locker)) {
    timer->setInterval(pollIntervalMs);
    connect(timer, &QTimer::timeout, this, &SerialCircularRequester::processNext);
    connect(transport_.get(), &AbstractNetworkTransport::translateData, this, &SerialCircularRequester::translateData);
    connect(transport_.get(), SIGNAL(translateData(QByteArray)), this, SLOT(unlock(QByteArray)), Qt::UniqueConnection);
    connect(transport_.get(), &AbstractNetworkTransport::packetAccepted,
            this, &SerialCircularRequester::onPacketAccepted,
            Qt::UniqueConnection);
}

const AbstractNetworkTransport *SerialCircularRequester::getTransport()
{
    return transport_.get();
}
#endif

void SerialCircularRequester::addCircularCommand(AbstractCommand *cmd) {
    if (!cmd || circular_commands_.contains(cmd)) {
        return;
    }

    circular_commands_.append(cmd);
}

void SerialCircularRequester::addDisposableCommand(AbstractCommand *cmd) {
    // Команды устройств являются переиспользуемыми объектами. Повторное
    // добавление того же указателя до отправки не должно создавать несколько
    // одинаковых записей, каждая из которых всё равно увидит последнее value.
    if (cmd && !disposable_commands_.contains(cmd)) {
        disposable_commands_.enqueue(cmd);
    }
}

void SerialCircularRequester::addNoResponceCommand(AbstractCommand *cmd)
{
    if (!cmd) {
        return;
    }

    if (current_is_no_response_ && current_cmd_ == cmd) {
        repeat_current_no_response_ = true;
        return;
    }

    if (!noresponce_commands_.contains(cmd)) {
        noresponce_commands_.enqueue(cmd);
    }
}

void SerialCircularRequester::removeCircularCommand(AbstractCommand *cmd)
{
    const qsizetype index = circular_commands_.indexOf(cmd);
    if (index < 0) {
        return;
    }

    circular_commands_.removeAt(index);
    disposable_commands_.removeAll(cmd);
    if (index < read_index_) {
        --read_index_;
    }
    if (read_index_ < 0 || read_index_ >= circular_commands_.size()) {
        read_index_ = 0;
    }

    if (current_cmd_ == cmd && state_ != RequestState::Idle) {
        delete_current_when_idle_ = true;
    } else {
        delete cmd;
        if (current_cmd_ == cmd) {
            current_cmd_ = nullptr;
        }
    }
}

void SerialCircularRequester::removeCommands() {
    const AbstractCommand *activeCommand = current_cmd_.data();
    const bool activeIsCircular =
        activeCommand && circular_commands_.contains(current_cmd_.data());

    for (AbstractCommand *command : std::as_const(circular_commands_)) {
        if (command != activeCommand) {
            delete command;
        }
    }
    circular_commands_.clear();
    read_index_ = 0;

    if (activeIsCircular) {
        if (state_ == RequestState::Idle) {
            delete current_cmd_.data();
            current_cmd_ = nullptr;
        } else {
            // Активная команда удалится после ответа или таймаута.
            delete_current_when_idle_ = true;
        }
    }
}

void SerialCircularRequester::startRequest() {
    timer->start();
}

void SerialCircularRequester::stopRequest()
{
    timer->stop();
}

void SerialCircularRequester::processNext() {
    if (state_ == RequestState::WaitingForWrite) {
        return;
    }
    if (state_ == RequestState::WaitingForResponse) {
        if (response_timer_.isValid() &&
            response_timer_.elapsed() < locker_->timeout()) {
            return;
        }

        qWarning() << "Command response timeout";
        locker_->unlock();
        finishCurrentCommand();
    }

    if (locker_->isLocked()) {
        return;
    }

    while (!disposable_commands_.isEmpty() &&
           disposable_commands_.head() == nullptr) {
        disposable_commands_.dequeue();
    }

    while (!noresponce_commands_.isEmpty() &&
           noresponce_commands_.head() == nullptr) {
        noresponce_commands_.dequeue();
    }

    const bool hasResponseCommands =
        !disposable_commands_.isEmpty() ||
        !circular_commands_.isEmpty();

    const bool takeNoResponse =
        !noresponce_commands_.isEmpty() &&
        (prefer_no_response_ || !hasResponseCommands);

    if (takeNoResponse) {
        current_cmd_ = noresponce_commands_.head();

        current_is_no_response_ = true;
        current_is_disposable_ = false;
    } else {
        current_is_no_response_ = false;

        const bool takeDisposable =
            !disposable_commands_.isEmpty() &&
            (prefer_disposable_ || circular_commands_.isEmpty());

        if (takeDisposable) {
            current_cmd_ = disposable_commands_.head();
            current_is_disposable_ = true;
        } else if (!circular_commands_.isEmpty()) {
            if (read_index_ >= circular_commands_.size()) {
                read_index_ = 0;
            }

            current_cmd_ = circular_commands_.at(read_index_);
            current_is_disposable_ = false;
        } else {
            current_cmd_ = nullptr;
            return;
        }
    }

    if (!current_cmd_) {
        if (!current_is_disposable_) {
            read_index_ = (read_index_ + 1) % circular_commands_.size();
        }
        return;
    }

    pending_packet_ = current_cmd_->makeCommand();
    early_response_buffer_.clear();
    if (pending_packet_.isEmpty()) {
        qWarning() << "Command produced an empty packet";
        rejectCurrentCommand();
        return;
    }

    state_ = RequestState::WaitingForWrite;

#ifdef MYABSTRACTCONNECT_H
    pending_packet_id_ = 1;
    connect_->writeData(pending_packet_);
    onPacketAccepted(pending_packet_id_, pending_packet_);
#else
    pending_packet_id_ = transport_->writeTracked(pending_packet_);
    if (pending_packet_id_ == 0) {
        state_ = RequestState::Idle;
        pending_packet_.clear();
        current_cmd_ = nullptr;
    }
#endif
}

void SerialCircularRequester::onPacketAccepted(
    quint64 packetId,
    const QByteArray &packet)
{
    if (state_ != RequestState::WaitingForWrite ||
        packetId != pending_packet_id_ ||
        packet != pending_packet_) {
        return;
    }

    if (current_is_no_response_) {
        if (!noresponce_commands_.isEmpty() &&
            noresponce_commands_.head() == current_cmd_.data()) {
            noresponce_commands_.dequeue();
        }

        if (repeat_current_no_response_) {
            repeat_current_no_response_ = false;
            noresponce_commands_.enqueue(current_cmd_.data());
        }

        prefer_no_response_ = false;

        finishCurrentCommand();
        return;
    }

    if (current_is_disposable_) {
        if (!disposable_commands_.isEmpty() &&
            disposable_commands_.head() == current_cmd_.data()) {
            disposable_commands_.dequeue();
        }

        prefer_disposable_ = false;
    } else if (!circular_commands_.isEmpty()) {
        read_index_ =
            (read_index_ + 1) % circular_commands_.size();

        prefer_disposable_ = true;
    }

    prefer_no_response_ = true;

    state_ = RequestState::WaitingForResponse;

    pending_packet_id_ = 0;
    pending_packet_.clear();

    locker_->lock();
    response_timer_.restart();

    if (!early_response_buffer_.isEmpty()) {
        const QByteArray earlyResponse =
            std::move(early_response_buffer_);

        unlock(earlyResponse);
    }
}

void SerialCircularRequester::unlock(QByteArray data) {
    if (state_ == RequestState::WaitingForWrite) {
        early_response_buffer_.append(data);
        return;
    }

    if (state_ != RequestState::WaitingForResponse) {
        return;
    }

    if (!current_cmd_ ||
        !response_timer_.isValid() ||
        response_timer_.elapsed() >= locker_->timeout()) {
        locker_->unlock();
        finishCurrentCommand();
        return;
    }

    if (current_cmd_->tryParse(data)) {
        locker_->unlock();
        finishCurrentCommand();
    }
}

void SerialCircularRequester::rejectCurrentCommand()
{
    if (current_is_no_response_) {
        if (!noresponce_commands_.isEmpty() &&
            noresponce_commands_.head() == current_cmd_.data()) {
            noresponce_commands_.dequeue();
        }

        prefer_no_response_ = false;
    } else if (current_is_disposable_) {
        if (!disposable_commands_.isEmpty() &&
            disposable_commands_.head() == current_cmd_.data()) {
            disposable_commands_.dequeue();
        }
        prefer_disposable_ = false;
        prefer_no_response_ = true;
    } else if (!circular_commands_.isEmpty()) {
        read_index_ = (read_index_ + 1) % circular_commands_.size();
        prefer_disposable_ = true;
        prefer_no_response_ = true;
    }

    finishCurrentCommand();
}

void SerialCircularRequester::finishCurrentCommand()
{
    AbstractCommand *finishedCommand = current_cmd_.data();

    state_ = RequestState::Idle;
    pending_packet_id_ = 0;
    pending_packet_.clear();
    early_response_buffer_.clear();
    current_cmd_ = nullptr;

    current_is_disposable_ = false;
    current_is_no_response_ = false;

    response_timer_.invalidate();

    if (delete_current_when_idle_) {
        delete_current_when_idle_ = false;
        delete finishedCommand;
    }
}
