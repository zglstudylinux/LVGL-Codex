#include "app_controller.h"
#include "app_ui.h"
#include "lvgl_codex/app.h"

static app_controller_t controller;

static void render(void)
{
    app_view_model_t view_model = app_controller_get_view_model(&controller);
    app_ui_render(&view_model);
}

static void handle_action(app_ui_action_t action, void * context)
{
    (void)context;
    app_controller_handle_action(&controller, action);
    render();
}

static lvgl_codex_result_t start(void)
{
    app_controller_init(&controller);
    app_ui_create(handle_action, NULL);
    render();
    return LVGL_CODEX_RESULT_OK;
}

const lvgl_codex_app_t * lvgl_codex_app_descriptor(void)
{
    static const lvgl_codex_app_t app = {
        .id = "__APP_NAME__",
        .start = start,
        .stop = NULL,
    };
    return &app;
}
