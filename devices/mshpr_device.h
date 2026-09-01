#ifndef MSHPRDEVICE_H
#define MSHPRDEVICE_H

#include <QObject>
#include "requesters/serial_circular_requester.h"
#include "cmd/abstract_command.h"
#include "parsers/mshpr_parser.h"

/**
 * @brief
 *
 */
struct MShPR_COMMANDS {
    inline static const QString STATUS_CMD = "STA"; /**< TODO: describe */
    inline static const QString GETERODIN_CMD = "FKA"; /**< TODO: describe */
    inline static const QString ATTENUATION_CMD = "AT1"; /**< TODO: describe */
    inline static const QString ADDRESS_CMD = "ADR"; /**< TODO: describe */
    inline static const QString BAUD_CMD = "BRG"; /**< TODO: describe */
};


/**
 * @brief
 *
 */
enum MShPRBaud {
    Baud4800 = 0,
    Baud9600 = 1,
    Baud19200 = 2,
    Baud38400 = 3
};


/**
 * @brief
 *
 */
class MShPRDevice : public QObject {
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
explicit MShPRDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent = nullptr);
    /**
     * @brief
     *
     */
void loadConfig();
    /**
     * @brief
     *
     * @return quint8
     */
quint8 getAttenuation();
    /**
     * @brief
     *
     * @return quint8
     */
quint8 getGeterodin();
    /**
     * @brief
     *
     * @return quint8
     */
quint8 getStatus();
    /**
     * @brief
     *
     * @return quint8
     */
quint8 getDeviceAddress();
    /**
     * @brief
     *
     * @return QString
     */
QString getLastAnswer();
    /**
     * @brief
     *
     * @param value
     */
void setAttenuation(quint8 value);
    /**
     * @brief
     *
     * @param value
     */
void setGeterodin(quint8 value);
    /**
     * @brief
     *
     * @param value
     */
void setAddress(quint8 value);
    /**
     * @brief
     *
     * @param baud
     */
void setBaud(MShPRBaud baud);
    /**
     * @brief
     *
     * @param baud
     */
void setBaud(quint8 baud);

signals:
private:


    QString m_configPath; /**< TODO: describe */
    QString m_section; /**< TODO: describe */
    SerialCircularRequester *m_requester; /**< TODO: describe */
    MShPRParser *m_parser; /**< TODO: describe */
    AbstractCommand *StatusCommand, *AttenuationCommand, *GeterodinCommand, *AddressCommand, *BaudCommand; /**< TODO: describe */
    QString m_deviceAddr = "01"; /**< TODO: describe */
    quint8 m_status = 0; /**< TODO: describe */
    quint8 m_attenuation = 0; /**< TODO: describe */
    quint8 m_geterodin = 0; /**< TODO: describe */
    quint8 m_baud = MShPRBaud::Baud19200; /**< TODO: describe */
    QString m_lastAnswer; /**< TODO: describe */
    QTimer *m_timer; /**< TODO: describe */
private slots:
    /**
     * @brief
     *
     * @param addr
     * @param QMap<QString
     * @param fieldsMap
     */
void processData(QString addr, QMap<QString, int> fieldsMap);
    /**
     * @brief
     *
     */
void onTimer();
    void LastAnswer(QByteArray packet);
};

#endif // MSHPRDEVICE_H
