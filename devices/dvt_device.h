#ifndef DVT_DEVICE_H
#define DVT_DEVICE_H

#include <QObject>
#include <QTimer>
#include <modbus/abstract_modbus_device.h>

/**
 * @brief
 *
 */
struct DvtState {
    quint8 status; /**< TODO: describe */
    float pressure; /**< TODO: describe */
    float tempetature; /**< TODO: describe */
    float humidity; /**< TODO: describe */
};

/**
 * @brief
 *
 */
class DvtDevice : public AbstractModbusDevice
{

    AbstractCommand *dvtStateCommand; /**< TODO: describe */
    quint8 deviceAddress; /**< TODO: describe */
    DvtState dvtInfo{}; /**< TODO: describe */
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
explicit DvtDevice(SerialCircularRequester *requester, QString configPath, QString section, QObject *parent = nullptr);
    /**
     * @brief
     *
     * @return quint8
     */
quint8 status();
    /**
     * @brief
     *
     * @return float
     */
float pressure();
    /**
     * @brief
     *
     * @return float
     */
float temperature();
    /**
     * @brief
     *
     * @return float
     */
float humidity();
signals:
private slots:
    /**
     * @brief
     *
     */
void onTimer();
    /**
     * @brief
     *
     * @param packet
     */
void LastAnswer(QByteArray packet);
};

#endif // DVT_DEVICE_H
