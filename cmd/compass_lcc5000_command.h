#ifndef COMPASSLCC5000COMMAND_H
#define COMPASSLCC5000COMMAND_H

#include "abstract_command.h"

const quint8 HEADERER = 0x77; /**< TODO: describe */

/**
 * @brief
 *
 */
class CompassLCC5000Command : public AbstractCommand {
    quint8 m_deviceAddr; /**< TODO: describe */
    quint8 m_cmdId; /**< TODO: describe */
    quint8 m_length; /**< TODO: describe */
    ValueType valueType; /**< TODO: describe */
    CommandType cmdType; /**< TODO: describe */
    QByteArray cachedRead; /**< TODO: describe */
    QByteArray cachedWrite; /**< TODO: describe */
    QByteArray responseBuffer; /**< TODO: describe */
    QVariant data; /**< TODO: describe */
public:
    /**
     * @brief
     *
     * @param deviceAddr
     * @param cmdId
     * @param length
     * @param valueType
     * @param cmdType
     */
explicit CompassLCC5000Command(quint8 deviceAddr, quint8 cmdId, quint8 length, ValueType valueType, CommandType cmdType);
    /**
     * @brief
     *
     * @return const QByteArray
     */
const QByteArray &makeReadCommand();
    /**
     * @brief
     *
     * @return const QByteArray
     */
const QByteArray &makeWriteCommand();

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
     * @param data
     * @return bool
     */
bool tryParse(const QByteArray &data) override;

    // AbstractCommand interface
public:
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
};

#endif // COMPASSLCC5000COMMAND_H
