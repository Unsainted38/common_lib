#ifndef ABSTRACTCOMMAND_H
#define ABSTRACTCOMMAND_H

#include <QObject>
#include <QIODevice>
#include <QDataStream>
#include <QDebug>
#include <QList>
#include <QVariant>

/**
 * @brief Определяет тип значения, кодируемого или декодируемого командой.
 */
enum ValueType {
    QINT8 = 7,
    QUINT8 = 8,
    QUINT16 = 16,
    QUINT32 = 32,
    QINT32 = 31,
    QSTRING = 25,
    FLOAT = 26,
    DOUBLE = 27,
    QBYTEARRAY = 28,
    QLISTQUINT16 = 29,
};

/**
 * @brief Определяет направление и семейство команды устройства.
 */
enum class CommandType {
    READ,
    WRITE,
    MODBUS,
    READWRITE,
};

/**
 * @brief Задаёт общий интерфейс команды устройства и разбора её ответа.
 */
class AbstractCommand : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Задаёт общий интерфейс команды устройства и разбора её ответа.
     *
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit AbstractCommand(QObject *parent = nullptr);
    /**
     * @brief Формирует пакет команды и сбрасывает буфер ожидаемого ответа.
     *
     * @return Сформированный массив байтов.
     */
virtual const QByteArray &makeCommand() = 0;

    /**
     * @brief Возвращает последнее принятое или установленное значение команды.
     *
     * @return Значение в контейнере QVariant.
     */
virtual QVariant getValue();
    /**
     * @brief Устанавливает значение для последующего формирования команды записи.
     *
     * @param v Новое значение команды.
     */
virtual void setValue(QVariant v);
    /**
     * @brief Возвращает признак успешного ответа на команду записи.
     *
     * @return Текущее логическое состояние.
     */
virtual bool isSuccess();

    /**
     * @brief Добавляет фрагмент ответа в буфер и проверяет завершённые кадры.
     *
     * @param data Входные данные или полезная нагрузка ответа.
     * @return true, если найден и обработан полный корректный кадр.
     */
virtual bool tryParse(const QByteArray &data);

public slots:
    /**
     * @brief Декодирует полезную нагрузку ответа и обновляет значение команды.
     *
     * @param data Входные данные или полезная нагрузка ответа.
     * @param regAddr Адрес регистра.
     */
virtual void processData(const QByteArray &data, quint16 regAddr);
    /**
     * @brief Декодирует полезную нагрузку ответа и обновляет значение команды.
     *
     * @param data Входные данные или полезная нагрузка ответа.
     */
virtual void processData(const QByteArray &data);
signals:
protected:
};

#endif // ABSTRACTCOMMAND_H
