#ifndef UBPCHCOMMAND_H
#define UBPCHCOMMAND_H

#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <QEventLoop>

#include "abstract_command.h"

const quint16 HEADER = 0xFEFE; /**< TODO: describe */
const quint8 DST = 0x01; // адрес получателя /**< TODO: describe */
const quint8 SRC = 0x16; // адрес отправителя /**< TODO: describe */
const quint8 readCmd = 0x03; /**< TODO: describe */
const quint8 writeCmd = 0x05; /**< TODO: describe */
const quint16 END = 0xFCFC; /**< TODO: describe */



/**
 * @brief
 *
 */
class UBPChCommand : public AbstractCommand {
    Q_OBJECT

public:
    /**
     * @brief
     *
     * @param deviceAddr
     * @param masterAddr
     * @param regAddr
     * @param valueType
     * @param cmdType
     */
explicit UBPChCommand(quint8 deviceAddr, quint8 masterAddr, quint16 regAddr, ValueType valueType, CommandType cmdType)
        :
        m_regAddr(regAddr),
        m_deviceAddr(deviceAddr),
        m_masterAddr(masterAddr),
        cmdType(cmdType),
        valueType(valueType)
    {
    }
    /**
     * @brief
     *
     * @param data
     * @param regAddr
     */
void processData(const QByteArray &data, quint16 regAddr) override;
signals:
private:
    quint16 m_regAddr; /**< TODO: describe */
    quint8 m_deviceAddr; /**< TODO: describe */
    quint8 m_masterAddr; /**< TODO: describe */
    CommandType cmdType; /**< TODO: describe */
    ValueType valueType; /**< TODO: describe */
    QVariant value; /**< TODO: describe */
    /**
     * @brief
     *
     * @param frame
     * @return QByteArray
     */
static QByteArray pack(const QByteArray &frame);
    QByteArray cachedRead; /**< TODO: describe */
    QByteArray cachedWrite; /**< TODO: describe */
    QByteArray responseBuffer; /**< TODO: describe */
    /**
     * @brief
     *
     * @return const QByteArray
     */
const QByteArray &makeReadCommand();
    /**
     * @brief
     *
     * @return const QByteArray
     */
const QByteArray &makeWriteCommand();
    /**
     * @brief
     *
     * @param frame
     * @return QByteArray
     */
static QByteArray unpack(const QByteArray &frame);
private slots:
    //void onDataReady(QByteArray data, quint16 regAddr);

    // AbstractCommand interface
public:
    /**
     * @brief
     *
     * @return const QByteArray
     */
const QByteArray &makeCommand() override;
    /**
     * @brief
     *
     * @param data
     * @return bool
     */
bool tryParse(const QByteArray &data) override;

    // AbstractCommand interface
public:
    /**
     * @brief
     *
     * @return QVariant
     */
QVariant getValue() override;
    /**
     * @brief
     *
     * @param v
     */
void setValue(QVariant v) override;
};

#endif // UBPCHCOMMAND_H
