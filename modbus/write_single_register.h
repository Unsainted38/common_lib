#ifndef WRITE_SINGLE_REGISTER_H
#define WRITE_SINGLE_REGISTER_H

#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>



/**
 * @brief Реализует Modbus-функцию 0x06 Write Single Register.
 */
class WriteSingleRegister : public AbstractCommand
{
    Q_OBJECT
    AbstractModBusProtocol *protocol; /**< Реализация упаковки и разбора Modbus. */
    QByteArray cachedCommand; /**< Кэш последнего сформированного пакета. */
    QByteArray cachedPdu; /**< Кэш PDU без транспортной обёртки. */
    quint16 registerAddress; /**< Начальный адрес регистра. */
    quint16 reg = 0; /**< Значение reg. */
    quint16 cachedReg = 0; /**< Хранит cached reg. */
    QByteArray buffer; /**< Накопительный буфер входных данных. */
    bool commandStatus = false; /**< Признак успешного подтверждения команды записи. */
    const quint8 cmdID = 0x06; /**< Код функции протокола. */
public:
    /**
     * @brief Реализует Modbus-функцию 0x06 Write Single Register.
     *
     * @param regAddress Начальный адрес регистра.
     * @param protocol Реализация транспортного формата Modbus.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit WriteSingleRegister(quint16 regAddress, AbstractModBusProtocol *protocol, QObject *parent = nullptr);

    // AbstractCommand interface
public:
    /**
     * @brief Формирует пакет команды и сбрасывает буфер ожидаемого ответа.
     *
     * @return Сформированный массив байтов.
     */
const QByteArray &makeCommand() override;
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

#endif // WRITE_SINGLE_REGISTER_H
