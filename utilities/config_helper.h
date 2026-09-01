#ifndef CONFIGHELPER_H
#define CONFIGHELPER_H

#include <QObject>
#include <QSettings>

/**
 * @brief Читает общие параметры устройства из INI-файла.
 */
class ConfigHelper : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Читает общие параметры устройства из INI-файла.
     *
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit ConfigHelper(QObject *parent = nullptr);
    /**
     * @brief Загружает общие параметры транспорта из выбранной секции.
     *
     * @param path Путь к INI-файлу конфигурации.
     * @param section Имя секции с параметрами объекта.
     */
static void loadTransportConfig(QString path, QString section);
    /**
     * @brief Загружает адрес Modbus-устройства из выбранной секции.
     *
     * @param path Путь к INI-файлу конфигурации.
     * @param section Имя секции с параметрами объекта.
     * @return Результат операции типа quint8.
     */
static quint8 loadModBusDeviceAddress(QString path, QString section);


signals:

};

#endif // CONFIGHELPER_H
