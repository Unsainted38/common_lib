#ifndef MSHPRCOMMAND_H
#define MSHPRCOMMAND_H

#include <QObject>
#include <QIODevice>
#include <QDataStream>
#include <QMap>
#include "abstract_command.h"


/**
 * @brief Формирует текстовые команды протокола МШПР и проверяет ответы.
 */
class MShPRCommand : public AbstractCommand {
    Q_OBJECT
    QString m_addr; /**< Хранит addr. */
    QString m_cmd; /**< Хранит cmd. */
    QString data; /**< Текущее значение или полезная нагрузка. */
    QByteArray cachedRead; /**< Кэш пакета команды чтения. */
    QByteArray cachedWrite; /**< Кэш пакета команды записи. */
    QByteArray responseBuffer; /**< Накопительный буфер ответа активной команды. */
    CommandType cmdType; /**< Тип операции команды. */
    ValueType valueType; /**< Тип кодируемого значения. */

    /**
     * @brief Формирует пакет записи из текущего значения.
     *
     * @return Сформированный массив байтов.
     */
const QByteArray &makeWriteCommand();
    /**
     * @brief Формирует и кэширует пакет чтения.
     *
     * @return Сформированный массив байтов.
     */
const QByteArray &makeReadCommand();
public:
    /**
     * @brief Формирует текстовые команды протокола МШПР и проверяет ответы.
     *
     * @param addr Адрес устройства в формате протокола.
     * @param cmd Строковый идентификатор команды.
     * @param cmdType Тип операции чтения или записи.
     */
explicit MShPRCommand(QString addr, QString cmd, CommandType cmdType);


    /**
     * @brief Устанавливает address.
     *
     * @param addr Адрес устройства в формате протокола.
     */
void setAddress(QString addr);
signals:


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

#endif // MSHPRCOMMAND_H
