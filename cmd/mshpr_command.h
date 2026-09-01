#ifndef MSHPRCOMMAND_H
#define MSHPRCOMMAND_H

#include <QObject>
#include <QIODevice>
#include <QDataStream>
#include <QMap>
#include "abstract_command.h"


/**
 * @brief
 *
 */
class MShPRCommand : public AbstractCommand {
    Q_OBJECT
    QString m_addr; /**< TODO: describe */
    QString m_cmd; /**< TODO: describe */
    QString data; /**< TODO: describe */
    QByteArray cachedRead; /**< TODO: describe */
    QByteArray cachedWrite; /**< TODO: describe */
    QByteArray responseBuffer; /**< TODO: describe */
    CommandType cmdType; /**< TODO: describe */
    ValueType valueType; /**< TODO: describe */

    /**
     * @brief
     *
     * @return const QByteArray
     */
const QByteArray &makeWriteCommand();
    /**
     * @brief
     *
     * @return const QByteArray
     */
const QByteArray &makeReadCommand();
public:
    /**
     * @brief
     *
     * @param addr
     * @param cmd
     * @param cmdType
     */
explicit MShPRCommand(QString addr, QString cmd, CommandType cmdType);


    /**
     * @brief
     *
     * @param addr
     */
void setAddress(QString addr);
signals:


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

#endif // MSHPRCOMMAND_H
