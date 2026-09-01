#ifndef UM2_13_COMMNAD_H
#define UM2_13_COMMNAD_H

#include <QObject>
#include "abstract_command.h"

/**
 * @brief Формирует и разбирает бинарные команды устройства УМ2-13.
 */
class UM2_13Commnad : public AbstractCommand
{
    Q_OBJECT
    quint16 cmdId; /**< Код команды протокола. */
    quint16 length; /**< Хранит length. */
    CommandType cmdType; /**< Тип операции команды. */
    ValueType valueType; /**< Тип кодируемого значения. */
    QVariant value; /**< Текущее значение команды. */
    QVariant cachedValue; /**< Значение cached value. */
    QByteArray cachedCommand; /**< Кэш последнего сформированного пакета. */
    QByteArray buffer; /**< Накопительный буфер входных данных. */
public:
    /**
     * @brief Формирует и разбирает бинарные команды устройства УМ2-13.
     *
     * @param cmdId Код команды протокола.
     * @param length Ожидаемая полная длина ответа в байтах.
     * @param valueType Тип кодируемого значения.
     * @param cmdType Тип операции чтения или записи.
     */
explicit UM2_13Commnad(quint16 cmdId, quint16 length, ValueType valueType, CommandType cmdType);

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
     * @brief Устанавливает значение для последующего формирования команды записи.
     *
     * @param v Новое значение команды.
     */
void setValue(QVariant v) override;
    /**
     * @brief Добавляет фрагмент ответа в буфер и проверяет завершённые кадры.
     *
     * @param data Входные данные или полезная нагрузка ответа.
     * @return true, если найден и обработан полный корректный кадр.
     */
bool tryParse(const QByteArray &data) override;
signals:
    /**
     * @brief Передаёт полный корректный ответ подписчикам.
     */
void translateLastAnswer(QByteArray);
};

#endif // UM2_13_COMMNAD_H
