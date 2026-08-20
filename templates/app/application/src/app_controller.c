#include "app_controller.h"

void app_controller_init(app_controller_t * controller)
{
    app_state_init(&controller->state);
}

void app_controller_handle_action(app_controller_t * controller, app_ui_action_t action)
{
    if(action == APP_UI_ACTION_TOGGLE) {
        app_state_toggle(&controller->state);
    }
}

app_view_model_t app_controller_get_view_model(const app_controller_t * controller)
{
    app_view_model_t view_model = {.enabled = controller->state.enabled};
    return view_model;
}
