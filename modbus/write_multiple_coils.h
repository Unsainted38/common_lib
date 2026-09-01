#ifndef WRITE_MULTIPLE_COILS_H
#define WRITE_MULTIPLE_COILS_H

#include <QObject>
#include <abstract_command.h>
#include <modbus/abstract_modbus_protocol.h>


/**
 * @brief Реализует Modbus-функцию 0x0F Write Multiple Coils.
 */
class WriteMultipleCoils : public AbstractCommand
{
    Q_OBJECT
    AbstractModBusProtocol *protocol; /**< Реализация упаковки и разбора Modbus. */
    QByteArray cachedCommand; /**< Кэш последнего сформированного пакета. */
    QByteArray cachedPdu; /**< Кэш PDU без транспортной обёртки. */
    quint16 coilAddress; /**< Начальный адрес катушки. */
    quint16 coilsCount; /**< Количество катушек в операции. */
    QVector<quint8> coils; /**< Последние прочитанные или заданные состояния катушек. */
    QVector<quint8> cachedCoils; /**< Хранит cached coils. */
    QByteArray buffer; /**< Накопительный буфер входных данных. */
    bool commandStatus = false; /**< Признак успешного подтверждения команды записи. */
    const quint8 cmdID = 0x0F; /**< Код функции протокола. */
public:
    /**
     * @brief Реализует Modbus-функцию 0x0F Write Multiple Coils.
     *
     * @param coilAddress Начальный адрес катушки.
     * @param coilsCount Количество катушек.
     * @param protocol Реализация транспортного формата Modbus.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit WriteMultipleCoils(quint16 coilAddress, quint16 coilsCount, AbstractModBusProtocol *protocol, QObject *parent = nullptr);

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

#endif // WRITE_MULTIPLE_COILS_H
