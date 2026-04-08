#ifndef CONFIGHELPER_H
#define CONFIGHELPER_H

#include <QObject>
#include <QSettings>

class ConfigHelper : public QObject {
    Q_OBJECT

public:
    explicit ConfigHelper(QObject *parent = nullptr);
    static void loadTransportConfig(QString path, QString section);
    static quint8 loadModBusDeviceAddress(QString path, QString section);


signals:

};

#endif // CONFIGHELPER_H
