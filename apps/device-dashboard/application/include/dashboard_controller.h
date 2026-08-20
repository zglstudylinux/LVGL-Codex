#ifndef DEVICE_DASHBOARD_CONTROLLER_H
#define DEVICE_DASHBOARD_CONTROLLER_H

#include "device_service.h"
#include "dashboard_ui.h"

typedef struct {
    device_service_t device_service;
} dashboard_controller_t;

void dashboard_controller_init(dashboard_controller_t * controller);
void dashboard_controller_handle_action(dashboard_controller_t * controller, dashboard_ui_action_t action);
dashboard_view_model_t dashboard_controller_get_view_model(const dashboard_controller_t * controller);

#endif /* DEVICE_DASHBOARD_CONTROLLER_H */
