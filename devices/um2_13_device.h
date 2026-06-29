#ifndef UM2_13_DEVICE_H
#define UM2_13_DEVICE_H

#include <QObject>
#include "requesters/serial_circular_requester.h"

struct StatusInfo {
    quint8 State;
    quint16 L1PwrIn;
    quint16 L1PwrOutF;
    quint16 L1PwrOutR;
    quint16 L1TempAmp;
    quint16 L3PwrIn;
    quint16 L3PwrOutF;
    quint16 L3PwrOutR;
    quint16 L3TempAmp;
    quint16 L5PwrIn;
    quint16 L5PwrOutF;
    quint16 L5PwrOutR;
    quint16 L5TempAmp;
};

struct ErrorsInfo {
    quint8 Switch;
    quint16 Main;
    quint16 Rsrv;
};

class UM2_13Device : public QObject
{
    Q_OBJECT
    QString m_configPath;
    QString m_section;
    SerialCircularRequester *m_requester;
    AbstractCommand *RStatus, *RErrors, *RL1Attenuation, *RL3Attenuation, *RL5Attenuation, *RPathSelect,
                    *WL1Attenuation, *WL3Attenuation, *WL5Attenuation, *WPathSelect;
    StatusInfo Status;
    ErrorsInfo Errors;
    QTimer *m_timer;
    QByteArray m_lastAnswer;
    bool m_statusOnline = false;

public:
    explicit UM2_13Device(SerialCircularRequester *requester, QObject *parent = nullptr);
    float L1Attenuation();
    float L3Attenuation();
    float L5Attenuation();
    StatusInfo status();
    ErrorsInfo errors();
    void setL1Attenuation(float attenuation);
    void setL3Attenuation(float attenuation);
    void setL5Attenuation(float attenuation);
signals:
private slots:
    void onTimer();
    void LastAnswer(QByteArray packet);
};

#endif // UM2_13_DEVICE_H
