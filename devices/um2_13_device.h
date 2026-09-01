#ifndef UM2_13_DEVICE_H
#define UM2_13_DEVICE_H

#include <QObject>
#include "requesters/serial_circular_requester.h"

/**
 * @brief
 *
 */
struct StatusInfo {
    quint8 State; /**< TODO: describe */
    quint16 L1PwrIn; /**< TODO: describe */
    quint16 L1PwrOutF; /**< TODO: describe */
    quint16 L1PwrOutR; /**< TODO: describe */
    quint16 L1TempAmp; /**< TODO: describe */
    quint16 L3PwrIn; /**< TODO: describe */
    quint16 L3PwrOutF; /**< TODO: describe */
    quint16 L3PwrOutR; /**< TODO: describe */
    quint16 L3TempAmp; /**< TODO: describe */
    quint16 L5PwrIn; /**< TODO: describe */
    quint16 L5PwrOutF; /**< TODO: describe */
    quint16 L5PwrOutR; /**< TODO: describe */
    quint16 L5TempAmp; /**< TODO: describe */
};

/**
 * @brief
 *
 */
struct ErrorsInfo {
    quint8 Switch; /**< TODO: describe */
    quint16 Main; /**< TODO: describe */
    quint16 Rsrv; /**< TODO: describe */
};

/**
 * @brief
 *
 */
class UM2_13Device : public QObject
{
    Q_OBJECT
    QString m_configPath; /**< TODO: describe */
    QString m_section; /**< TODO: describe */
    SerialCircularRequester *m_requester; /**< TODO: describe */
    AbstractCommand *RStatus, *RErrors, *RL1Attenuation, *RL3Attenuation, *RL5Attenuation, *RPathSelect, /**< TODO: describe */
                    *WL1Attenuation, *WL3Attenuation, *WL5Attenuation, *WPathSelect; /**< TODO: describe */
    StatusInfo Status{}; /**< TODO: describe */
    ErrorsInfo Errors{}; /**< TODO: describe */
    QTimer *m_timer; /**< TODO: describe */
    QByteArray m_lastAnswer; /**< TODO: describe */
    bool m_statusOnline = false; /**< TODO: describe */

public:
    /**
     * @brief
     *
     * @param requester
     * @param parent
     */
explicit UM2_13Device(SerialCircularRequester *requester, QObject *parent = nullptr);
    /**
     * @brief
     *
     * @return float
     */
float L1Attenuation();
    /**
     * @brief
     *
     * @return float
     */
float L3Attenuation();
    /**
     * @brief
     *
     * @return float
     */
float L5Attenuation();
    /**
     * @brief
     *
     * @return StatusInfo
     */
StatusInfo status();
    /**
     * @brief
     *
     * @return ErrorsInfo
     */
ErrorsInfo errors();
    /**
     * @brief
     *
     * @param attenuation
     */
void setL1Attenuation(float attenuation);
    /**
     * @brief
     *
     * @param attenuation
     */
void setL3Attenuation(float attenuation);
    /**
     * @brief
     *
     * @param attenuation
     */
void setL5Attenuation(float attenuation);
signals:
private slots:
    /**
     * @brief
     *
     */
void onTimer();
    void LastAnswer(QByteArray packet);
};

#endif // UM2_13_DEVICE_H
