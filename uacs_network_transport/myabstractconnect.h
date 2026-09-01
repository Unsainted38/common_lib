#ifndef MYABSTRACTCONNECT_H
#define MYABSTRACTCONNECT_H

#include <QObject>
#include <QByteArray>

/**
 * @brief
 *
 */
class MyAbstractConnect : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
explicit MyAbstractConnect(QObject *parent = nullptr);

    /**
     * @brief
     *
     * @param data
     */
virtual void writeData(const QByteArray &data) = 0;


public slots:
    /**
     * @brief
     *
     */
virtual void onReadyRead() = 0;
    /**
     * @brief
     *
     * @param bytes
     */
virtual void handleBytesWritten(qint64 bytes) = 0;
    /**
     * @brief
     *
     */
virtual void writeNext() = 0;

signals:
    /**
     * @brief
     *
     * @param QByteArray
     */
void readyToProcessData(QByteArray);
};

#endif // MYABSTRACTCONNECT_H
