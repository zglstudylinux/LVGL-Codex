#ifndef DEVICE_DASHBOARD_UI_H
#define DEVICE_DASHBOARD_UI_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    DASHBOARD_UI_ACTION_TOGGLE_OUTPUT,
} dashboard_ui_action_t;

typedef void (*dashboard_ui_action_handler_t)(dashboard_ui_action_t action, void * context);

typedef struct {
    bool output_enabled;
    uint16_t temperature_celsius;
    uint8_t battery_percent;
} dashboard_view_model_t;

void dashboard_ui_create(dashboard_ui_action_handler_t action_handler, void * context);
void dashboard_ui_render(const dashboard_view_model_t * view_model);

#endif /* DEVICE_DASHBOARD_UI_H */
