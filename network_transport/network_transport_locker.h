#ifndef NETWORKTRANSPORT_LOCKER_H
#define NETWORKTRANSPORT_LOCKER_H

#include <QElapsedTimer>

#include <stdint.h>

class NetworkTransportLocker {
public:
    NetworkTransportLocker(
        uint16_t timeout,
        uint16_t safeTimeout) :
        m_locked(false),
        m_timeout(timeout),
        m_safeTimeout(safeTimeout) {
        m_lockedTime.start();
    }
    bool isLocked() {
        if(m_locked) {
            if(m_lockedTime.elapsed() >= m_timeout) {
                m_locked = false;
            }
        } else {
            if(m_lockedTime.elapsed() <= m_safeTimeout) {
                return true;
            } else {
                m_locked = false;
            }
        }

        return m_locked;
    }
    void lock() {
        m_locked = true;
        m_lockedTime.start();
    }
    void unlock() {
        m_locked = false;
        m_lockedTime.start();
    }
    void setTimeout(uint16_t timeout) {
        m_timeout = timeout;
        m_lockedTime.restart();
    }
    uint16_t timeout() {
        return m_timeout;
    }
protected:
    bool m_locked;
    uint16_t m_timeout;
    uint16_t m_safeTimeout;
    QElapsedTimer m_lockedTime;
};

#endif // NETWORKTRANSPORT_LOCKER_H
