#include "dashboard_ui.h"

#include "lvgl_compat.h"

#include <stdio.h>

static lv_obj_t * output_value_label;
static lv_obj_t * temperature_value_label;
static lv_obj_t * battery_value_label;
static dashboard_ui_action_handler_t action_handler;
static void * action_context;

static void output_button_event_cb(lv_event_t * event)
{
    if(lv_event_get_code(event) == LV_EVENT_CLICKED && action_handler != NULL) {
        action_handler(DASHBOARD_UI_ACTION_TOGGLE_OUTPUT, action_context);
    }
}

static lv_obj_t * create_value_row(lv_obj_t * parent, const char * title)
{
    lv_obj_t * row = lv_obj_create(parent);
    lv_obj_set_width(row, LV_PCT(100));
    lv_obj_set_height(row, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(row, 12, 0);
    lv_obj_set_style_border_width(row, 0, 0);
    lv_obj_set_layout(row, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * label = lv_label_create(row);
    lv_label_set_text(label, title);
    return lv_label_create(row);
}

void dashboard_ui_create(dashboard_ui_action_handler_t new_action_handler, void * context)
{
    action_handler = new_action_handler;
    action_context = context;

    lv_obj_t * screen = lvgl_codex_active_screen();
    lv_obj_set_style_bg_color(screen, lv_palette_darken(LV_PALETTE_BLUE_GREY, 4), 0);

    lv_obj_t * panel = lv_obj_create(screen);
    lv_obj_set_size(panel, LV_PCT(92), LV_PCT(86));
    lv_obj_center(panel);
    lv_obj_set_layout(panel, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(panel, 20, 0);

    lv_obj_t * title = lv_label_create(panel);
    lv_label_set_text(title, "Embedded Device Dashboard");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);

    temperature_value_label = create_value_row(panel, "Temperature");
    battery_value_label = create_value_row(panel, "Battery");
    output_value_label = create_value_row(panel, "Output");

    lv_obj_t * button = lv_button_create(panel);
    lv_obj_set_width(button, LV_PCT(100));
    lv_obj_add_event_cb(button, output_button_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * button_label = lv_label_create(button);
    lv_label_set_text(button_label, "Toggle output");
    lv_obj_center(button_label);
}

void dashboard_ui_render(const dashboard_view_model_t * view_model)
{
    char text[32];

    (void)snprintf(text, sizeof(text), "%u C", view_model->temperature_celsius);
    lv_label_set_text(temperature_value_label, text);

    (void)snprintf(text, sizeof(text), "%u %%", view_model->battery_percent);
    lv_label_set_text(battery_value_label, text);

    lv_label_set_text(output_value_label, view_model->output_enabled ? "ON" : "OFF");
}
