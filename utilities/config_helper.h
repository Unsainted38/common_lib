#ifndef CONFIGHELPER_H
#define CONFIGHELPER_H

#include <QObject>
#include <QSettings>

template<typename T, typename M>
struct ConfigField
{
    QString key;
    M T::* member;
};

template<typename T, typename M>
ConfigField<T, M> field(
    QString key,
    M T::* member)
{
    return {
        std::move(key),
        member
    };
}

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

template<typename T, typename... Fields>
static T loadConfig(QSettings& settings, Fields&&... fields) {
    T result{};

    (loadField(settings, result, std::forward<Fields>(fields)), ...);

    return result;
}

private:

template<typename T, typename Field>
static void loadField(
    QSettings& settings,
    T& result,
    const Field&& field)
{
    using ValueType = std::remove_cvref_t<decltype(result.*(field.member))>;
    result.*(field.member) = settings.value(field.key).template value<ValueType>();
}

};

#endif // CONFIGHELPER_H
