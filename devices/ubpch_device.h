#ifndef UBPCHDEVICE_H
#define UBPCHDEVICE_H

#include <QObject>
#include "requesters/serial_circular_requester.h"
#include "cmd/abstract_command.h"
#include "parsers/ubpch_parser.h"
#include <QTimer>

/**
 * @brief
 *
 */
class UBPChDevice : public QObject {
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param requester
     * @param configPath
     * @param section
     * @param parent
     */
explicit UBPChDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent = nullptr);
    /**
     * @brief
     *
     */
void loadConfig();
    /**
     * @brief
     *
     * @param value
     */
void setEnabled(quint8 value);
    /**
     * @brief
     *
     * @return quint8
     */
quint8 getEnabled();
    /**
     * @brief
     *
     * @param value
     */
void setAttenuation(quint8 value);
    /**
     * @brief
     *
     * @return quint32
     */
quint32 getAttenuation();
    /**
     * @brief
     *
     * @return quint8
     */
quint8 getErrorStatus();
    /**
     * @brief
     *
     * @return quint8
     */
quint8 getDeviceStatus();
    /**
     * @brief
     *
     * @return float
     */
float getOutputPower();
    /**
     * @brief
     *
     * @return float
     */
float getEmpower();
    /**
     * @brief
     *
     * @return qint32
     */
qint32 getTemperature();
    /**
     * @brief
     *
     * @return float
     */
float getVoltage();
    /**
     * @brief
     *
     * @return bool
     */
bool getStatusOnline();

    /**
     * @brief
     *
     * @return QByteArray
     */
QByteArray getLastAnswer();
signals:
private:
    QString m_configPath; /**< TODO: describe */
    QString m_section; /**< TODO: describe */
    SerialCircularRequester *m_requester; /**< TODO: describe */
    UBPChParser *m_parser; /**< TODO: describe */
    AbstractCommand *EnableCommand, *AttenuationCommand, *ErrorStatusCommand, *DeviceStatusCommand, /**< TODO: describe */
                    *OutputPowerCommand, *EmpowerCommand, *TemperatureCommand, *VoltageCommand; /**< TODO: describe */
    AbstractCommand *EnableWriteCommand, *AttenuationWriteCommand; /**< TODO: describe */
    quint32 m_attenuation; /**< TODO: describe */
    quint8 m_enabled; /**< TODO: describe */
    quint8 m_deviceStatus; /**< TODO: describe */
    quint8 m_errorStatus; /**< TODO: describe */
    quint32 m_outputPower; /**< TODO: describe */
    quint32 m_empower; /**< TODO: describe */
    qint32 m_temperature; /**< TODO: describe */
    float m_voltage; /**< TODO: describe */
    quint8 m_deviceAddr = 0x16; /**< TODO: describe */
    quint8 m_masterAddr = 0x01; /**< TODO: describe */
    QEventLoop loop; /**< TODO: describe */
    QTimer *m_timer; /**< TODO: describe */
    bool m_statusOnline = false; /**< TODO: describe */
    QByteArray m_lastAnswer; /**< TODO: describe */
private slots:
    /**
     * @brief
     *
     */
void onTimer();
    /**
     * @brief
     *
     */
void onStatusOnline();
public slots:
    void onLastAnswer(QByteArray packet);
};

#endif // UBPCHDEVICE_H
