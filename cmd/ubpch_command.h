#ifndef UBPCHCOMMAND_H
#define UBPCHCOMMAND_H

#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <QEventLoop>

#include "abstract_command.h"

const quint16 HEADER = 0xFEFE;
const quint8 DST = 0x01; // адрес получателя
const quint8 SRC = 0x16; // адрес отправителя
const quint8 readCmd = 0x03;
const quint8 writeCmd = 0x05;
const quint16 END = 0xFCFC;



class UBPChCommand : public AbstractCommand {
    Q_OBJECT
public:
    explicit UBPChCommand(quint8 deviceAddr, quint8 masterAddr, quint16 regAddr, ValueType type)
        : AbstractCommand(type),
        m_regAddr(regAddr),
        m_deviceAddr(deviceAddr),
        m_masterAddr(masterAddr) {

    }
    const QByteArray &makeReadCommand() override;
    const QByteArray &makeWriteCommand() override;
    void processData(const QByteArray &data, quint16 regAddr) override;
signals:
private:
    quint16 m_regAddr;
    quint8 m_deviceAddr;
    quint8 m_masterAddr;
    static QByteArray pack(const QByteArray &frame);
    QByteArray cachedRead;
    QByteArray cachedWrite;
private slots:
    //void onDataReady(QByteArray data, quint16 regAddr);
};

#endif // UBPCHCOMMAND_H
