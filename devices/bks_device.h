#ifndef BKSDEVICE_H
#define BKSDEVICE_H

#include <QObject>
#include <QTimer>

#include "requesters/serial_circular_requester.h"
#include "cmd/abstract_command.h"
#include "parsers/mshpr_parser.h"

/**
 * @brief
 *
 */
enum BksBaud {
    Baud4800 = 0,
    Baud9600 = 1,
    Baud19200 = 2,
    Baud38400 = 3
};


/**
 * @brief
 *
 */
struct BKS_COMMANDS {
    inline static const QString STATUS_CMD = "STA"; /**< TODO: describe */
    inline static const QString FC_LOW_FREQ_CMD = "FC1"; /**< TODO: describe */
    inline static const QString FC_HIGH_FREQ_CMD = "FC2"; /**< TODO: describe */
    inline static const QString FX_LOW_FREQ_CMD = "FX1"; /**< TODO: describe */
    inline static const QString FX_HIGH_FREQ_CMD = "FX2"; /**< TODO: describe */
    inline static const QString ADDRESS_CMD = "ADR"; /**< TODO: describe */
    inline static const QString BAUD_CMD = "BRG"; /**< TODO: describe */
};
/**
 * @brief
 *
 */
class BksDevice : public QObject {
    Q_OBJECT

    QString m_configPath; /**< TODO: describe */
    QString m_section; /**< TODO: describe */
    SerialCircularRequester *m_requester; /**< TODO: describe */
    MShPRParser *m_parser; /**< TODO: describe */
    AbstractCommand *StatusCommand, *FC1Command, *FC2Command, *FX1Command, *FX2Command, *AddressCommand, *BaudCommand; /**< TODO: describe */
    QString m_deviceAddr = "00"; /**< TODO: describe */
    bool m_statusOnline = false; /**< TODO: describe */
    quint8 m_FC1 = 0; /**< TODO: describe */
    quint8 m_FC2 = 0; /**< TODO: describe */
    quint8 m_FX1 = 0; /**< TODO: describe */
    quint8 m_FX2 = 0; /**< TODO: describe */
    quint8 m_baud = BksBaud::Baud19200; /**< TODO: describe */
    QString m_lastAnswer; /**< TODO: describe */
    QTimer *m_timer; /**< TODO: describe */
public:
    /**
     * @brief
     *
     * @param requester
     * @param configPath
     * @param section
     * @param parent
     */
explicit BksDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent = nullptr);
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
quint8 getFC1();
    /**
     * @brief
     *
     * @return quint8
     */
quint8 getFC2();
    /**
     * @brief
     *
     * @return quint8
     */
quint8 getFX1();
    /**
     * @brief
     *
     * @return quint8
     */
quint8 getFX2();
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
     * @return bool
     */
bool getStatusOnline();
    /**
     * @brief
     *
     * @param FC1
     */
void setFC1(quint8 FC1);
    /**
     * @brief
     *
     * @param FC2
     */
void setFC2(quint8 FC2);
    /**
     * @brief
     *
     * @param FX1
     */
void setFX1(quint8 FX1);
    /**
     * @brief
     *
     * @param FX2
     */
void setFX2(quint8 FX2);
    /**
     * @brief
     *
     * @param value
     */
void setDeviceAddress(quint8 value);
    /**
     * @brief
     *
     * @param baud
     */
void setBaud(BksBaud baud);
    /**
     * @brief
     *
     * @param baud
     */
void setBaud(quint8 baud);
signals:

private:

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

#endif // BKSDEVICE_H
