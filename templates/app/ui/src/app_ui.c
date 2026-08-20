#include "app_ui.h"

#include "lvgl_compat.h"

static lv_obj_t * value_label;
static app_ui_action_handler_t action_handler;
static void * action_context;

static void button_event_cb(lv_event_t * event)
{
    if(lv_event_get_code(event) == LV_EVENT_CLICKED && action_handler != NULL) {
        action_handler(APP_UI_ACTION_TOGGLE, action_context);
    }
}

void app_ui_create(app_ui_action_handler_t handler, void * context)
{
    action_handler = handler;
    action_context = context;

    lv_obj_t * screen = lvgl_codex_active_screen();
    lv_obj_t * title = lv_label_create(screen);
    lv_label_set_text(title, "__APP_NAME__");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 32);

    value_label = lv_label_create(screen);
    lv_obj_center(value_label);

    lv_obj_t * button = lv_button_create(screen);
    lv_obj_align(button, LV_ALIGN_BOTTOM_MID, 0, -32);
    lv_obj_add_event_cb(button, button_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t * label = lv_label_create(button);
    lv_label_set_text(label, "Toggle");
    lv_obj_center(label);
}

void app_ui_render(const app_view_model_t * view_model)
{
    lv_label_set_text(value_label, view_model->enabled ? "Enabled" : "Disabled");
}
