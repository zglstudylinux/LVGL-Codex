#include "dashboard_controller.h"
#include "dashboard_ui.h"
#include "lvgl_codex/app.h"

#include <stddef.h>

static dashboard_controller_t controller;

static void render_current_state(void)
{
    dashboard_view_model_t view_model = dashboard_controller_get_view_model(&controller);
    dashboard_ui_render(&view_model);
}

static void handle_ui_action(dashboard_ui_action_t action, void * context)
{
    (void)context;
    dashboard_controller_handle_action(&controller, action);
    render_current_state();
}

static lvgl_codex_result_t device_dashboard_start(void)
{
    dashboard_controller_init(&controller);
    dashboard_ui_create(handle_ui_action, NULL);
    render_current_state();
    return LVGL_CODEX_RESULT_OK;
}

static void device_dashboard_stop(void)
{
}

const lvgl_codex_app_t * lvgl_codex_app_descriptor(void)
{
    static const lvgl_codex_app_t app = {
        .id = "device-dashboard",
        .start = device_dashboard_start,
        .stop = device_dashboard_stop,
    };
    return &app;
}
