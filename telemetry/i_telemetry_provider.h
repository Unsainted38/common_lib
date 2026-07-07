#ifndef I_TELEMETRY_PROVIDER_H
#define I_TELEMETRY_PROVIDER_H

template <typename TData>
class ITelemetryProvider
{
public:
    virtual ~ITelemetryProvider() = default;

    virtual TData currentData() const = 0;
};

#endif // I_TELEMETRY_PROVIDER_H
