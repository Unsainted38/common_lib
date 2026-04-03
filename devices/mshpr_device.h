#ifndef MSHPRDEVICE_H
#define MSHPRDEVICE_H

#include <QObject>
#include "requesters/serial_circular_requester.h"
#include "cmd/abstract_command.h"
#include "parsers/mshpr_parser.h"
#include "network_transport/abstract_network_transport.h"

struct MShPR_COMMANDS {
    inline static const QString STATUS_CMD = "STA";
    inline static const QString GETERODIN_CMD = "FKA";
    inline static const QString ATTENUATION_CMD = "AT1";
    inline static const QString ADDRESS_CMD = "ADR";
    inline static const QString BAUD_CMD = "BRG";
};


enum MShPRBaud {
    Baud4800 = 0,
    Baud9600 = 1,
    Baud19200 = 2,
    Baud38400 = 3
};


class MShPRDevice : public QObject {
    Q_OBJECT
public:
    explicit MShPRDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent = nullptr);
    void loadConfig();
    quint8 getAttenuation();
    quint8 getGeterodin();
    quint8 getStatus();
    quint8 getDeviceAddress();
    QString getLastAnswer();
    void setAttenuation(quint8 value);
    void setGeterodin(quint8 value);
    void setAddress(quint8 value);
    void setBaud(MShPRBaud baud);
    void setBaud(quint8 baud);

signals:
private:


    QString m_configPath;
    QString m_section;
    SerialCircularRequester *m_requester;
    MShPRParser *m_parser;
    AbstractNetworkTransport *m_transport;
    AbstractCommand *StatusCommand, *AttenuationCommand, *GeterodinCommand, *AddressCommand, *BaudCommand;
    QString m_deviceAddr = "01";
    quint8 m_status = 0;
    quint8 m_attenuation = 0;
    quint8 m_geterodin = 0;
    quint8 m_baud = MShPRBaud::Baud19200;
    QString m_lastAnswer;
    QTimer *m_timer;
private slots:
    void processData(QString addr, QMap<QString, int> fieldsMap);
    void onTimer();
    void LastAnswer(QByteArray packet);
};

#endif // MSHPRDEVICE_H
