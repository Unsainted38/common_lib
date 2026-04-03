#ifndef UBPCHDEVICE_H
#define UBPCHDEVICE_H

#include <QObject>
#include "network_transport/abstract_network_transport_factory.h"
#include "requesters/serial_circular_requester.h"
#include "cmd/ubpch_command.h"
#include "cmd/abstract_command.h"
#include "parsers/ubpch_parser.h"
#include <QCoreApplication>
#include <QtEndian>
#include <type_traits>
#include <QTimer>

class UBPChDevice : public QObject {
    Q_OBJECT
public:
    explicit UBPChDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent = nullptr);
    void loadConfig();
    void setEnabled(quint8 value);
    quint8 getEnabled();
    void setAttenuation(quint8 value);
    quint32 getAttenuation();
    quint8 getErrorStatus();
    quint8 getDeviceStatus();
    float getOutputPower();
    float getEmpower();
    qint32 getTemperature();
    float getVoltage();
    bool getStatusOnline();

    QByteArray getLastAnswer();
signals:
private:
    QString m_configPath;
    QString m_section;
    SerialCircularRequester *m_requester;
    UBPChParser *m_parser;
    AbstractNetworkTransport *m_transport;
    AbstractCommand *EnableCommand, *AttenuationCommand, *ErrorStatusCommand, *DeviceStatusCommand,
                    *OutputPowerCommand, *EmpowerCommand, *TemperatureCommand, *VoltageCommand;
    quint32 m_attenuation;
    quint8 m_enabled;
    quint8 m_deviceStatus;
    quint8 m_errorStatus;
    quint32 m_outputPower;
    quint32 m_empower;
    qint32 m_temperature;
    float m_voltage;
    quint8 m_deviceAddr = 0x16;
    quint8 m_masterAddr = 0x01;
    QEventLoop loop;
    QTimer *m_timer;
    bool m_statusOnline = false;
    QByteArray m_lastAnswer;
private slots:
    void onTimer();
    void onStatusOnline();
public slots:
    void onLastAnswer(QByteArray packet);
};

#endif // UBPCHDEVICE_H
