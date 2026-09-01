#ifndef I_TELEMETRY_PROVIDER_H
#define I_TELEMETRY_PROVIDER_H

template <typename TData>
/**
 * @brief Интерфейс получения текущего снимка телеметрических данных.
 */
class ITelemetryProvider
{
public:
    /**
     * @brief Освобождает ресурсы объекта.
     */
virtual ~ITelemetryProvider() = default;

    /**
     * @brief Возвращает текущий снимок телеметрии.
     *
     * @return Текущее значение параметра.
     */
virtual TData currentData() const = 0;
};

#endif // I_TELEMETRY_PROVIDER_H
