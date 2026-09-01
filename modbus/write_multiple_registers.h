#ifndef WRITE_MULTIPLE_REGISTERS_H
#define WRITE_MULTIPLE_REGISTERS_H

#include <QObject>
#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>


/**
 * @brief Реализует Modbus-функцию 0x10 Write Multiple Registers.
 */
class WriteMultipleRegisters : public AbstractCommand
{
    AbstractModBusProtocol *protocol; /**< Реализация упаковки и разбора Modbus. */
    QByteArray cachedCommand; /**< Кэш последнего сформированного пакета. */
    QByteArray cachedPdu; /**< Кэш PDU без транспортной обёртки. */
    quint16 registerAddress; /**< Начальный адрес регистра. */
    quint16 registersCount; /**< Количество регистров в операции. */
    QByteArray buffer; /**< Накопительный буфер входных данных. */
    QVector<quint16> regs; /**< Последние прочитанные или заданные значения регистров. */
    QVector<quint16> cachedRegs; /**< Хранит cached regs. */
    bool commandStatus = false; /**< Признак успешного подтверждения команды записи. */
    const quint8 cmdID = 0x10; /**< Код функции протокола. */
public:
    /**
     * @brief Реализует Modbus-функцию 0x10 Write Multiple Registers.
     *
     * @param regAddress Начальный адрес регистра.
     * @param regsCount Количество запрашиваемых регистров.
     * @param protocol Реализация транспортного формата Modbus.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit WriteMultipleRegisters(quint16 regAddress, quint16 regsCount, AbstractModBusProtocol *protocol, QObject *parent = nullptr);

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
     * @brief Устанавливает значение для последующего формирования команды записи.
     *
     * @param v Новое значение команды.
     */
void setValue(QVariant v) override;
    /**
     * @brief Возвращает признак успешного ответа на команду записи.
     *
     * @return Текущее логическое состояние.
     */
bool isSuccess() override;
    /**
     * @brief Добавляет фрагмент ответа в буфер и проверяет завершённые кадры.
     *
     * @param data Входные данные или полезная нагрузка ответа.
     * @return true, если найден и обработан полный корректный кадр.
     */
bool tryParse(const QByteArray &data) override;
};

#endif // WRITE_MULTIPLE_REGISTERS_H
