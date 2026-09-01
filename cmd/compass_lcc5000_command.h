#ifndef COMPASSLCC5000COMMAND_H
#define COMPASSLCC5000COMMAND_H

#include "abstract_command.h"

const quint8 HEADERER = 0x77; /**< Хранит headerer. */

/**
 * @brief Формирует команды и распознаёт ответы компаса LC-C5000.
 */
class CompassLCC5000Command : public AbstractCommand {
    quint8 m_deviceAddr; /**< Адрес device addr. */
    quint8 m_cmdId; /**< Код команды протокола. */
    quint8 m_length; /**< Хранит length. */
    ValueType valueType; /**< Тип кодируемого значения. */
    CommandType cmdType; /**< Тип операции команды. */
    QByteArray cachedRead; /**< Кэш пакета команды чтения. */
    QByteArray cachedWrite; /**< Кэш пакета команды записи. */
    QByteArray responseBuffer; /**< Накопительный буфер ответа активной команды. */
    QVariant data; /**< Текущее значение или полезная нагрузка. */
public:
    /**
     * @brief Формирует команды и распознаёт ответы компаса LC-C5000.
     *
     * @param deviceAddr Адрес управляемого устройства.
     * @param cmdId Код команды протокола.
     * @param length Длина протокольного блока от поля длины до CRC включительно.
     * @param valueType Тип кодируемого значения.
     * @param cmdType Тип операции чтения или записи.
     */
explicit CompassLCC5000Command(quint8 deviceAddr, quint8 cmdId, quint8 length, ValueType valueType, CommandType cmdType);
    /**
     * @brief Формирует и кэширует пакет чтения.
     *
     * @return Сформированный массив байтов.
     */
const QByteArray &makeReadCommand();
    /**
     * @brief Формирует пакет записи из текущего значения.
     *
     * @return Сформированный массив байтов.
     */
const QByteArray &makeWriteCommand();

    // AbstractCommand interface
public:
    /**
     * @brief Формирует пакет команды и сбрасывает буфер ожидаемого ответа.
     *
     * @return Сформированный массив байтов.
     */
const QByteArray &makeCommand() override;
    /**
     * @brief Добавляет фрагмент ответа в буфер и проверяет завершённые кадры.
     *
     * @param data Входные данные или полезная нагрузка ответа.
     * @return true, если найден и обработан полный корректный кадр.
     */
bool tryParse(const QByteArray &data) override;

    // AbstractCommand interface
public:
    /**
     * @brief Возвращает последнее принятое или установленное значение команды.
     *
     * @return Значение в контейнере QVariant.
     */
QVariant getValue() override;
    /**
     * @brief Устанавливает значение для последующего формирования команды записи.
     *
     * @param v Новое значение команды.
     */
void setValue(QVariant v) override;
};

#endif // COMPASSLCC5000COMMAND_H
