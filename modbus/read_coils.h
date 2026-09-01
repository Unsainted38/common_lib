#ifndef READ_COILS_H
#define READ_COILS_H

#include <QObject>
#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>


/**
 * @brief Реализует Modbus-функцию 0x01 Read Coils.
 */
class ReadCoils : public AbstractCommand
{
    Q_OBJECT
    QByteArray cachedCommand; /**< Кэш последнего сформированного пакета. */
    QByteArray cachedPdu; /**< Кэш PDU без транспортной обёртки. */
    AbstractModBusProtocol *protocol; /**< Реализация упаковки и разбора Modbus. */
    quint16 coilAddress; /**< Начальный адрес катушки. */
    quint16 coilsCount; /**< Количество катушек в операции. */
    quint8 byteCount; /**< Ожидаемое количество байтов данных в ответе. */
    QByteArray buffer; /**< Накопительный буфер входных данных. */
    QVector<quint8> coils; /**< Последние прочитанные или заданные состояния катушек. */
    const quint8 cmdID = 0x01; /**< Код функции протокола. */
public:
    /**
     * @brief Реализует Modbus-функцию 0x01 Read Coils.
     *
     * @param coilAddress Начальный адрес катушки.
     * @param coilsCount Количество катушек.
     * @param protocol Реализация транспортного формата Modbus.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit ReadCoils(quint16 coilAddress, quint16 coilsCount, AbstractModBusProtocol *protocol, QObject *parent = nullptr);

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


};

#endif // READ_COILS_H
