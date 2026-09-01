#ifndef WRITE_SINGLE_COIL_H
#define WRITE_SINGLE_COIL_H

#include <QObject>
#include <QVector>
#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>


/**
 * @brief Реализует Modbus-функцию 0x05 Write Single Coil.
 */
class WriteSingleCoil : public AbstractCommand
{
    Q_OBJECT
    bool coil = false; /**< Значение coil. */
    bool cachedCoil = false; /**< Хранит cached coil. */
    QByteArray cachedCommand; /**< Кэш последнего сформированного пакета. */
    QByteArray cachedPdu; /**< Кэш PDU без транспортной обёртки. */
    AbstractModBusProtocol *protocol; /**< Реализация упаковки и разбора Modbus. */
    quint16 coilAddress; /**< Начальный адрес катушки. */
    QByteArray buffer; /**< Накопительный буфер входных данных. */
    bool commandStatus = false; /**< Признак успешного подтверждения команды записи. */
    const quint8 cmdID = 0x05; /**< Код функции протокола. */
public:
    /**
     * @brief Реализует Modbus-функцию 0x05 Write Single Coil.
     *
     * @param coilAddress Начальный адрес катушки.
     * @param protocol Реализация транспортного формата Modbus.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit WriteSingleCoil(quint16 coilAddress, AbstractModBusProtocol *protocol, QObject *parent = nullptr);

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
     * @param boolean Логическое состояние катушки.
     */
void setValue(QVariant boolean) override;
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

#endif // WRITE_SINGLE_COIL_H
