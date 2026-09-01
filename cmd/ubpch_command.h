#ifndef UBPCHCOMMAND_H
#define UBPCHCOMMAND_H

#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <QEventLoop>

#include "abstract_command.h"

const quint16 HEADER = 0xFEFE; /**< Хранит header. */
const quint8 DST = 0x01; /**< Адрес получателя по умолчанию. */
const quint8 SRC = 0x16; /**< Адрес отправителя по умолчанию. */
const quint8 readCmd = 0x03; /**< Хранит read cmd. */
const quint8 writeCmd = 0x05; /**< Хранит write cmd. */
const quint16 END = 0xFCFC; /**< Хранит end. */



/**
 * @brief Формирует экранированные кадры УБПЧ и разбирает ответы устройства.
 */
class UBPChCommand : public AbstractCommand {
    Q_OBJECT

public:
    /**
     * @brief Формирует экранированные кадры УБПЧ и разбирает ответы устройства.
     *
     * @param deviceAddr Адрес управляемого устройства.
     * @param masterAddr Адрес ведущего устройства.
     * @param regAddr Адрес регистра.
     * @param valueType Тип кодируемого значения.
     * @param cmdType Тип операции чтения или записи.
     */
explicit UBPChCommand(quint8 deviceAddr, quint8 masterAddr, quint16 regAddr, ValueType valueType, CommandType cmdType)
        :
        m_regAddr(regAddr),
        m_deviceAddr(deviceAddr),
        m_masterAddr(masterAddr),
        cmdType(cmdType),
        valueType(valueType)
    {
    }
    /**
     * @brief Декодирует полезную нагрузку ответа и обновляет значение команды.
     *
     * @param data Входные данные или полезная нагрузка ответа.
     * @param regAddr Адрес регистра.
     */
void processData(const QByteArray &data, quint16 regAddr) override;
signals:
private:
    quint16 m_regAddr; /**< Адрес reg addr. */
    quint8 m_deviceAddr; /**< Адрес device addr. */
    quint8 m_masterAddr; /**< Адрес master addr. */
    CommandType cmdType; /**< Тип операции команды. */
    ValueType valueType; /**< Тип кодируемого значения. */
    QVariant value; /**< Текущее значение команды. */
    /**
     * @brief Упаковывает полезную нагрузку в кадр соответствующего протокола.
     *
     * @param frame Структура, в которую записывается разобранный кадр.
     * @return Сформированный массив байтов.
     */
static QByteArray pack(const QByteArray &frame);
    QByteArray cachedRead; /**< Кэш пакета команды чтения. */
    QByteArray cachedWrite; /**< Кэш пакета команды записи. */
    QByteArray responseBuffer; /**< Накопительный буфер ответа активной команды. */
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
    /**
     * @brief Удаляет байты экранирования из принятого кадра.
     *
     * @param frame Структура, в которую записывается разобранный кадр.
     * @return Сформированный массив байтов.
     */
static QByteArray unpack(const QByteArray &frame);
private slots:
    //void onDataReady(QByteArray data, quint16 regAddr);

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

#endif // UBPCHCOMMAND_H
