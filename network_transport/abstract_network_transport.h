#ifndef ABSTRACTNETWORKTRANSPORT_H
#define ABSTRACTNETWORKTRANSPORT_H

#include <QObject>
#include <QSettings>
#include <QMutex>
#include <QDebug>

/**
 * @brief
 *
 */
class AbstractNetworkTransport : public QObject {
    Q_OBJECT
public:
    /**
     * @brief
     *
     */
enum TErrorCode {
        OK,
        NOT_CONNECTED,
        WRITE_OK,
        WRITE_ERROR,
    };
    /**
     * @brief
     *
     * @param configPath
     * @param section
     * @param parent
     */
explicit AbstractNetworkTransport(QString configPath, QString section, QObject *parent = nullptr);
    /**
     * @brief
     *
     */
virtual void loadConfig() = 0;
    /**
     * @brief
     *
     */
virtual void setupTransport() = 0;
    /**
     * @brief
     *
     * @return bool
     */
virtual bool open() = 0;
    /**
     * @brief
     *
     * @param packet
     * @return bool
     */
virtual bool write(const QByteArray &packet) = 0;
    // Возвращает идентификатор поставленного в очередь пакета.
    // Ноль означает, что пакет не был принят транспортом.
    /**
     * @brief
     *
     * @param packet
     * @return quint64
     */
virtual quint64 writeTracked(const QByteArray &packet) = 0;
    /**
     * @brief
     *
     * @return bool
     */
virtual bool close() = 0;

signals:
    /**
     * @brief
     *
     * @param err
     * @param code
     */
void translateError(QString err, TErrorCode code);
    /**
     * @brief
     *
     * @param data
     */
void translateData(const QByteArray &data);
    // Все байты пакета переданы устройству транспорта. Это не подтверждение
    // получения или обработки пакета удалённой стороной.
    /**
     * @brief
     *
     * @param packetId
     * @param packet
     */
void packetAccepted(quint64 packetId, const QByteArray &packet);
protected:
    QString m_configPath; /**< TODO: describe */
    QString m_section; /**< TODO: describe */
protected slots:
    /**
     * @brief
     *
     */
virtual void processQueue() = 0;
    /**
     * @brief
     *
     */
virtual void heartbeat() = 0;
};

#endif // ABSTRACTNETWORKTRANSPORT_H
