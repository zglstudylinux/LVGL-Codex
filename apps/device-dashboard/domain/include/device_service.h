#ifndef DEVICE_DASHBOARD_DEVICE_SERVICE_H
#define DEVICE_DASHBOARD_DEVICE_SERVICE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool output_enabled;
    uint16_t temperature_celsius;
    uint8_t battery_percent;
} device_state_t;

typedef struct {
    device_state_t state;
} device_service_t;

void device_service_init(device_service_t * service);
void device_service_toggle_output(device_service_t * service);
device_state_t device_service_get_state(const device_service_t * service);

#endif /* DEVICE_DASHBOARD_DEVICE_SERVICE_H */
