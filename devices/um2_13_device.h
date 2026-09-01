#ifndef UM2_13_DEVICE_H
#define UM2_13_DEVICE_H

#include <QObject>
#include "requesters/serial_circular_requester.h"

/**
 * @brief Содержит показатели состояния каналов УМ2-13.
 */
struct StatusInfo {
    quint8 State; /**< Хранит state. */
    quint16 L1PwrIn; /**< Хранит l1 pwr in. */
    quint16 L1PwrOutF; /**< Хранит l1 pwr out f. */
    quint16 L1PwrOutR; /**< Хранит l1 pwr out r. */
    quint16 L1TempAmp; /**< Хранит l1 temp amp. */
    quint16 L3PwrIn; /**< Хранит l3 pwr in. */
    quint16 L3PwrOutF; /**< Хранит l3 pwr out f. */
    quint16 L3PwrOutR; /**< Хранит l3 pwr out r. */
    quint16 L3TempAmp; /**< Хранит l3 temp amp. */
    quint16 L5PwrIn; /**< Хранит l5 pwr in. */
    quint16 L5PwrOutF; /**< Хранит l5 pwr out f. */
    quint16 L5PwrOutR; /**< Хранит l5 pwr out r. */
    quint16 L5TempAmp; /**< Хранит l5 temp amp. */
};

/**
 * @brief Содержит маски ошибок УМ2-13.
 */
struct ErrorsInfo {
    quint8 Switch; /**< Хранит switch. */
    quint16 Main; /**< Хранит main. */
    quint16 Rsrv; /**< Хранит rsrv. */
};

/**
 * @brief Предоставляет опрос и настройку аттенюаторов устройства УМ2-13.
 */
class UM2_13Device : public QObject
{
    Q_OBJECT
    QString m_configPath; /**< Путь к INI-файлу конфигурации. */
    QString m_section; /**< Секция INI-файла для этого объекта. */
    SerialCircularRequester *m_requester; /**< Requester, выполняющий команды устройства. */
    AbstractCommand *RStatus, *RErrors, *RL1Attenuation, *RL3Attenuation, *RL5Attenuation, *RPathSelect, /**< Команды циклического чтения состояния УМ2-13. */
                    *WL1Attenuation, *WL3Attenuation, *WL5Attenuation, *WPathSelect; /**< Хранит wpath select. */
    StatusInfo Status{}; /**< Последний снимок состояния каналов. */
    ErrorsInfo Errors{}; /**< Последние принятые маски ошибок. */
    QTimer *m_timer; /**< Таймер контроля активности устройства. */
    QByteArray m_lastAnswer; /**< Последний выделенный ответ устройства. */
    bool m_statusOnline = false; /**< Признак недавнего корректного ответа устройства. */

public:
    /**
     * @brief Предоставляет опрос и настройку аттенюаторов устройства УМ2-13.
     *
     * @param requester Requester, выполняющий команды устройства.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit UM2_13Device(SerialCircularRequester *requester, QObject *parent = nullptr);
    /**
     * @brief Возвращает ослабление канала L1 в децибелах.
     *
     * @return Результат операции типа float.
     */
float L1Attenuation();
    /**
     * @brief Возвращает ослабление канала L3 в децибелах.
     *
     * @return Результат операции типа float.
     */
float L3Attenuation();
    /**
     * @brief Возвращает ослабление канала L5 в децибелах.
     *
     * @return Результат операции типа float.
     */
float L5Attenuation();
    /**
     * @brief Возвращает последний снимок состояния устройства.
     *
     * @return Текущее значение параметра.
     */
StatusInfo status();
    /**
     * @brief Возвращает последние маски ошибок устройства.
     *
     * @return Текущее значение параметра.
     */
ErrorsInfo errors();
    /**
     * @brief Устанавливает l1 attenuation.
     *
     * @param attenuation Требуемое ослабление в децибелах.
     */
void setL1Attenuation(float attenuation);
    /**
     * @brief Устанавливает l3 attenuation.
     *
     * @param attenuation Требуемое ослабление в децибелах.
     */
void setL3Attenuation(float attenuation);
    /**
     * @brief Устанавливает l5 attenuation.
     *
     * @param attenuation Требуемое ослабление в децибелах.
     */
void setL5Attenuation(float attenuation);
signals:
private slots:
    /**
     * @brief Обрабатывает событие timer.
     */
void onTimer();
    void LastAnswer(QByteArray packet);
};

#endif // UM2_13_DEVICE_H
