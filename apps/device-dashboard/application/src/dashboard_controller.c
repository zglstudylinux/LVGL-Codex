#include "dashboard_controller.h"

void dashboard_controller_init(dashboard_controller_t * controller)
{
    device_service_init(&controller->device_service);
}

void dashboard_controller_handle_action(dashboard_controller_t * controller, dashboard_ui_action_t action)
{
    if(action == DASHBOARD_UI_ACTION_TOGGLE_OUTPUT) {
        device_service_toggle_output(&controller->device_service);
    }
}

dashboard_view_model_t dashboard_controller_get_view_model(const dashboard_controller_t * controller)
{
    device_state_t state = device_service_get_state(&controller->device_service);
    dashboard_view_model_t view_model = {
        .output_enabled = state.output_enabled,
        .temperature_celsius = state.temperature_celsius,
        .battery_percent = state.battery_percent,
    };
    return view_model;
}
