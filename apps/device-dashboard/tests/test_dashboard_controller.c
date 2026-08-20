#include "dashboard_controller.h"

#include <assert.h>

int main(void)
{
    dashboard_controller_t controller;
    dashboard_controller_init(&controller);

    dashboard_view_model_t initial = dashboard_controller_get_view_model(&controller);
    assert(!initial.output_enabled);
    assert(initial.temperature_celsius == 25U);
    assert(initial.battery_percent == 86U);

    dashboard_controller_handle_action(&controller, DASHBOARD_UI_ACTION_TOGGLE_OUTPUT);
    dashboard_view_model_t updated = dashboard_controller_get_view_model(&controller);
    assert(updated.output_enabled);
    return 0;
}
