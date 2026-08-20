#include "device_service.h"

void device_service_init(device_service_t * service)
{
    service->state.output_enabled = false;
    service->state.temperature_celsius = 25U;
    service->state.battery_percent = 86U;
}

void device_service_toggle_output(device_service_t * service)
{
    service->state.output_enabled = !service->state.output_enabled;
}

device_state_t device_service_get_state(const device_service_t * service)
{
    return service->state;
}
