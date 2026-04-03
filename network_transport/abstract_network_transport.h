#ifndef ABSTRACTNETWORKTRANSPORT_H
#define ABSTRACTNETWORKTRANSPORT_H

#include <QObject>
#include <QSettings>
#include <QMutex>
#include <QDebug>

class AbstractNetworkTransport : public QObject {
    Q_OBJECT
public:
    enum TErrorCode {
        OK,
        NOT_CONNECTED,
        WRITE_OK,
        WRITE_ERROR,
    };
    explicit AbstractNetworkTransport(QString configPath, QString section, QObject *parent = nullptr);
    virtual void loadConfig() = 0;
    virtual void setupTransport() = 0;
    virtual bool open() = 0;
    virtual bool write(const QByteArray &packet) = 0;
    virtual bool close() = 0;

signals:
    void translateError(QString err, TErrorCode code);
    void translateData(const QByteArray &data);
protected:
    QString m_configPath;
    QString m_section;
protected slots:
    virtual void processQueue() = 0;
    virtual void heartbeat() = 0;
};

#endif // ABSTRACTNETWORKTRANSPORT_H
