#ifndef ABSTRACTNETWORKTRANSPORT_H
#define ABSTRACTNETWORKTRANSPORT_H

#include <QObject>
#include <QSettings>
#include <QMutex>
#include <QDebug>

/**
 * @brief Задаёт асинхронный интерфейс транспорта с очередью пакетов.
 */
class AbstractNetworkTransport : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Классифицирует события и ошибки транспорта.
     */
enum TErrorCode {
        OK,
        NOT_CONNECTED,
        WRITE_OK,
        WRITE_ERROR,
    };
    /**
     * @brief Задаёт асинхронный интерфейс транспорта с очередью пакетов.
     *
     * @param configPath Путь к INI-файлу конфигурации.
     * @param section Имя секции с параметрами объекта.
     * @param parent Родительский QObject, управляющий временем жизни объекта.
     */
explicit AbstractNetworkTransport(QString configPath, QString section, QObject *parent = nullptr);
    /**
     * @brief Загружает параметры из указанной секции INI-файла.
     */
virtual void loadConfig() = 0;
    /**
     * @brief Создаёт и настраивает объект ввода-вывода и таймеры транспорта.
     */
virtual void setupTransport() = 0;
    /**
     * @brief Открывает транспорт или запускает установление соединения.
     *
     * @return true, если операция была принята к выполнению.
     */
virtual bool open() = 0;
    /**
     * @brief Ставит пакет в очередь на отправку.
     *
     * @param packet Пакет данных для отправки.
     * @return true, если операция была принята к выполнению.
     */
virtual bool write(const QByteArray &packet) = 0;
    // Возвращает идентификатор поставленного в очередь пакета.
    // Ноль означает, что пакет не был принят транспортом.
    /**
     * @brief Ставит пакет в очередь и возвращает его уникальный идентификатор.
     *
     * @param packet Пакет данных для отправки.
     * @return Идентификатор пакета или 0 при отказе постановки в очередь.
     */
virtual quint64 writeTracked(const QByteArray &packet) = 0;
    /**
     * @brief Закрывает транспорт и отключает автоматическое переподключение.
     *
     * @return true, если операция была принята к выполнению.
     */
virtual bool close() = 0;

signals:
    /**
     * @brief Сообщает о результате операции транспорта.
     *
     * @param err Текст результата или ошибки.
     * @param code Классификация результата транспорта.
     */
void translateError(QString err, TErrorCode code);
    /**
     * @brief Передаёт принятые транспортом данные подписчикам.
     *
     * @param data Входные данные или полезная нагрузка ответа.
     */
void translateData(const QByteArray &data);
    // Все байты пакета переданы устройству транспорта. Это не подтверждение
    // получения или обработки пакета удалённой стороной.
    /**
     * @brief Сообщает, что все байты пакета переданы устройству транспорта.
     *
     * @param packetId Уникальный идентификатор пакета в очереди транспорта.
     * @param packet Пакет данных для отправки.
     */
void packetAccepted(quint64 packetId, const QByteArray &packet);
protected:
    QString m_configPath; /**< Путь к INI-файлу конфигурации. */
    QString m_section; /**< Секция INI-файла для этого объекта. */
protected slots:
    /**
     * @brief Продолжает отправку первого пакета очереди с учётом частичной записи.
     */
virtual void processQueue() = 0;
    /**
     * @brief Отправляет служебный пакет при активном соединении.
     */
virtual void heartbeat() = 0;
};

#endif // ABSTRACTNETWORKTRANSPORT_H
