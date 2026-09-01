#ifndef CONFIGHELPER_H
#define CONFIGHELPER_H

#include <QObject>
#include <QSettings>

/**
 * @brief
 *
 */
class ConfigHelper : public QObject {
    Q_OBJECT

public:
    /**
     * @brief
     *
     * @param parent
     */
explicit ConfigHelper(QObject *parent = nullptr);
    /**
     * @brief
     *
     * @param path
     * @param section
     */
static void loadTransportConfig(QString path, QString section);
    /**
     * @brief
     *
     * @param path
     * @param section
     * @return quint8
     */
static quint8 loadModBusDeviceAddress(QString path, QString section);


signals:

};

#endif // CONFIGHELPER_H
