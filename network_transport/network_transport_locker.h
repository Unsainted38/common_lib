#ifndef NETWORKTRANSPORT_LOCKER_H
#define NETWORKTRANSPORT_LOCKER_H

#include <QElapsedTimer>

#include <stdint.h>

/**
 * @brief Ограничивает параллельную отправку команд и выдерживает защитный интервал.
 */
class NetworkTransportLocker {
public:
    /**
     * @brief Ограничивает параллельную отправку команд и выдерживает защитный интервал.
     *
     * @param timeout Максимальное время ожидания ответа в миллисекундах.
     * @param safeTimeout Защитный интервал между командами в миллисекундах.
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
     * @brief Проверяет, запрещена ли отправка следующей команды.
     *
     * @return Текущее логическое состояние.
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
     * @brief Блокирует отправку новых команд на время ожидания ответа.
     */
void lock() {
        m_locked = true;
        m_lockedTime.start();
    }
    /**
     * @brief Передаёт принятые данные активной команде и завершает её при полном ответе.
     */
void unlock() {
        m_locked = false;
        m_lockedTime.start();
    }
    /**
     * @brief Изменяет максимальное время ожидания ответа.
     *
     * @param timeout Максимальное время ожидания ответа в миллисекундах.
     */
void setTimeout(uint16_t timeout) {
        m_timeout = timeout;
        m_lockedTime.restart();
    }
    /**
     * @brief Возвращает максимальное время ожидания ответа.
     *
     * @return Тайм-аут в миллисекундах.
     */
uint16_t timeout() {
        return m_timeout;
    }
protected:
    bool m_locked; /**< Хранит locked. */
    uint16_t m_timeout; /**< Хранит timeout. */
    uint16_t m_safeTimeout; /**< Хранит safe timeout. */
    QElapsedTimer m_lockedTime; /**< Хранит locked time. */
};

#endif // NETWORKTRANSPORT_LOCKER_H
