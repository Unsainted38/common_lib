#ifndef NETWORKTRANSPORT_LOCKER_H
#define NETWORKTRANSPORT_LOCKER_H

#include <QElapsedTimer>

#include <stdint.h>

/**
 * @brief
 *
 */
class NetworkTransportLocker {
public:
    /**
     * @brief
     *
     * @param timeout
     * @param safeTimeout
     */
    NetworkTransportLocker(
        uint16_t timeout,
        uint16_t safeTimeout) :
        m_locked(false),
        m_timeout(timeout),
        m_safeTimeout(safeTimeout) {
        m_lockedTime.start();
    }
    /**
     * @brief
     *
     * @return bool
     */
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
    /**
     * @brief
     *
     */
void lock() {
        m_locked = true;
        m_lockedTime.start();
    }
    /**
     * @brief
     *
     */
void unlock() {
        m_locked = false;
        m_lockedTime.start();
    }
    /**
     * @brief
     *
     * @param timeout
     */
void setTimeout(uint16_t timeout) {
        m_timeout = timeout;
        m_lockedTime.restart();
    }
    /**
     * @brief
     *
     * @return uint16_t
     */
uint16_t timeout() {
        return m_timeout;
    }
protected:
    bool m_locked; /**< TODO: describe */
    uint16_t m_timeout; /**< TODO: describe */
    uint16_t m_safeTimeout; /**< TODO: describe */
    QElapsedTimer m_lockedTime; /**< TODO: describe */
};

#endif // NETWORKTRANSPORT_LOCKER_H
