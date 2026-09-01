#ifndef UM2_13_COMMNAD_H
#define UM2_13_COMMNAD_H

#include <QObject>
#include "abstract_command.h"

/**
 * @brief
 *
 */
class UM2_13Commnad : public AbstractCommand
{
    Q_OBJECT
    quint16 cmdId; /**< TODO: describe */
    quint16 length; /**< TODO: describe */
    CommandType cmdType; /**< TODO: describe */
    ValueType valueType; /**< TODO: describe */
    QVariant value; /**< TODO: describe */
    QVariant cachedValue; /**< TODO: describe */
    QByteArray cachedCommand; /**< TODO: describe */
    QByteArray buffer; /**< TODO: describe */
public:
    /**
     * @brief
     *
     * @param cmdId
     * @param length
     * @param valueType
     * @param cmdType
     */
explicit UM2_13Commnad(quint16 cmdId, quint16 length, ValueType valueType, CommandType cmdType);

    // AbstractCommand interface
public:
    /**
     * @brief
     *
     * @return const QByteArray
     */
const QByteArray &makeCommand() override;
    /**
     * @brief
     *
     * @return QVariant
     */
QVariant getValue() override;
    /**
     * @brief
     *
     * @param v
     */
void setValue(QVariant v) override;
    /**
     * @brief
     *
     * @param data
     * @return bool
     */
bool tryParse(const QByteArray &data) override;
signals:
    /**
     * @brief
     *
     * @param QByteArray
     */
void translateLastAnswer(QByteArray);
};

#endif // UM2_13_COMMNAD_H
