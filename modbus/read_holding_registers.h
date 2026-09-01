#ifndef READ_HOLDING_REGISTERS_H
#define READ_HOLDING_REGISTERS_H

#include <QObject>
#include <QVector>
#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>


/**
 * @brief Реализует Modbus-функцию 0x03 Read Holding Registers.
 */
class ReadHoldingRegisters : public AbstractCommand
{
    QByteArray cachedCommand; /**< Кэш последнего сформированного пакета. */
    QByteArray cachedPdu; /**< Кэш PDU без транспортной обёртки. */
    AbstractModBusProtocol *protocol; /**< Реализация упаковки и разбора Modbus. */
    quint16 registerAddress; /**< Начальный адрес регистра. */
    quint16 registersCount; /**< Количество регистров в операции. */
    quint8 byteCount; /**< Ожидаемое количество байтов данных в ответе. */
    QByteArray buffer; /**< Накопительный буфер входных данных. */
    QVector<quint16> regs; /**< Последние прочитанные или заданные значения регистров. */
    const quint8 cmdID = 0x03; /**< Код функции протокола. */
public:
    /**
     * @brief Реализует Modbus-функцию 0x03 Read Holding Registers.
     *
     * @param regAddress Начальный адрес регистра.
     * @param regsCount Количество запрашиваемых регистров.
     * @param protocol Реализация транспортного формата Modbus.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit ReadHoldingRegisters(quint16 regAddress, quint16 regsCount, AbstractModBusProtocol *protocol, QObject *parent = nullptr);


    // AbstractCommand interface
public:
    /**
     * @brief Формирует пакет команды и сбрасывает буфер ожидаемого ответа.
     *
     * @return Сформированный массив байтов.
     */
const QByteArray &makeCommand() override;

    // AbstractCommand interface
public:
    /**
     * @brief Возвращает последнее принятое или установленное значение команды.
     *
     * @return Значение в контейнере QVariant.
     */
QVariant getValue() override;
    /**
     * @brief Добавляет фрагмент ответа в буфер и проверяет завершённые кадры.
     *
     * @param data Входные данные или полезная нагрузка ответа.
     * @return true, если найден и обработан полный корректный кадр.
     */
bool tryParse(const QByteArray &data) override;

public slots:
};

#endif // READ_HOLDING_REGISTERS_H
