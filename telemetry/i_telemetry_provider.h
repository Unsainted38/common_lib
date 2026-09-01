#ifndef I_TELEMETRY_PROVIDER_H
#define I_TELEMETRY_PROVIDER_H

template <typename TData>
/**
 * @brief
 *
 */
class ITelemetryProvider
{
public:
    /**
     * @brief
     *
     */
virtual ~ITelemetryProvider() = default;

    /**
     * @brief
     *
     * @return TData
     */
virtual TData currentData() const = 0;
};

#endif // I_TELEMETRY_PROVIDER_H
