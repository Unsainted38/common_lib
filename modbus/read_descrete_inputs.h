#ifndef READ_DESCRETE_INPUTS_H
#define READ_DESCRETE_INPUTS_H

#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>


/**
 * @brief Реализует Modbus-функцию 0x02 Read Discrete Inputs.
 */
class ReadDescreteInputs : public AbstractCommand
{
    Q_OBJECT
    AbstractModBusProtocol *protocol; /**< Реализация упаковки и разбора Modbus. */
    QByteArray cachedCommand; /**< Кэш последнего сформированного пакета. */
    QByteArray cachedPdu; /**< Кэш PDU без транспортной обёртки. */
    quint16 inputAddress; /**< Начальный адрес дискретного входа. */
    quint16 inputsCount; /**< Количество дискретных входов в операции. */
    quint8 byteCount; /**< Ожидаемое количество байтов данных в ответе. */
    QByteArray buffer; /**< Накопительный буфер входных данных. */
    QVector<quint8> inputs; /**< Последние прочитанные состояния дискретных входов. */
    const quint8 cmdID = 0x02; /**< Код функции протокола. */
public:
    /**
     * @brief Реализует Modbus-функцию 0x02 Read Discrete Inputs.
     *
     * @param inputAddress Начальный адрес дискретного входа.
     * @param inputsCount Количество дискретных входов.
     * @param protocol Реализация транспортного формата Modbus.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit ReadDescreteInputs(quint16 inputAddress, quint16 inputsCount, AbstractModBusProtocol *protocol, QObject *parent = nullptr);

    // AbstractCommand interface
public:
    /**
     * @brief Формирует пакет команды и сбрасывает буфер ожидаемого ответа.
     *
     * @return Сформированный массив байтов.
     */
const QByteArray &makeCommand() override;
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
};

#endif // READ_DESCRETE_INPUTS_H
