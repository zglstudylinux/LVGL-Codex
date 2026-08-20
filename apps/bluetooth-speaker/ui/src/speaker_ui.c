#include "speaker_ui.h"

#include "album_cover.h"
#include "speaker_font.h"
#include "lvgl_compat.h"

#include <stdint.h>
#include <stdio.h>

static lv_obj_t * connection_label;
static lv_obj_t * title_label;
static lv_obj_t * artist_label;
static lv_obj_t * elapsed_label;
static lv_obj_t * duration_label;
static lv_obj_t * playback_button_label;
static lv_obj_t * progress_bar;
static lv_obj_t * volume_slider;
static speaker_ui_action_handler_t action_handler;
static void * action_context;
static bool rendering;

static void dispatch_action(speaker_ui_action_type_t type, uint8_t value)
{
    if(action_handler != NULL) {
        speaker_ui_action_t action = { .type = type, .value = value };
        action_handler(&action, action_context);
    }
}

static void control_button_event_cb(lv_event_t * event)
{
    if(lv_event_get_code(event) == LV_EVENT_CLICKED) {
        dispatch_action((speaker_ui_action_type_t)(uintptr_t)lv_event_get_user_data(event), 0U);
    }
}

static void volume_slider_event_cb(lv_event_t * event)
{
    if(lv_event_get_code(event) == LV_EVENT_VALUE_CHANGED && !rendering) {
        dispatch_action(SPEAKER_UI_ACTION_SET_VOLUME, (uint8_t)lv_slider_get_value(volume_slider));
    }
}

static lv_obj_t * create_control_button(lv_obj_t * parent, const char * symbol, int32_t x, int32_t y,
                                         int32_t size, speaker_ui_action_type_t action, bool primary)
{
    lv_obj_t * button = lv_button_create(parent);
    lv_obj_set_size(button, size, size);
    lv_obj_set_pos(button, x, y);
    lv_obj_set_style_radius(button, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(button, primary ? lv_color_hex(0x32D6FF) : lv_color_hex(0x122A40), 0);
    lv_obj_set_style_shadow_width(button, primary ? 18 : 0, 0);
    lv_obj_set_style_shadow_color(button, lv_color_hex(0x1B9EC2), 0);
    lv_obj_add_event_cb(button, control_button_event_cb, LV_EVENT_CLICKED, (void *)(uintptr_t)action);

    lv_obj_t * label = lv_label_create(button);
    lv_label_set_text(label, symbol);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(label, primary ? lv_color_hex(0x03121C) : lv_color_hex(0xE6F6FF), 0);
    lv_obj_center(label);
    return label;
}

static void set_time_label(lv_obj_t * label, uint16_t seconds)
{
    char value[8];
    (void)snprintf(value, sizeof(value), "%u:%02u", seconds / 60U, seconds % 60U);
    lv_label_set_text(label, value);
}

void speaker_ui_create(speaker_ui_action_handler_t new_action_handler, void * context)
{
    action_handler = new_action_handler;
    action_context = context;

    lv_obj_t * screen = lvgl_codex_active_screen();
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x061525), 0);
    lv_obj_set_style_bg_grad_color(screen, lv_color_hex(0x0A2840), 0);
    lv_obj_set_style_bg_grad_dir(screen, LV_GRAD_DIR_VER, 0);

    lv_obj_t * status_dot = lv_obj_create(screen);
    lv_obj_set_size(status_dot, 10, 10);
    lv_obj_set_pos(status_dot, 50, 31);
    lv_obj_set_style_radius(status_dot, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(status_dot, lv_color_hex(0x46E6A7), 0);
    lv_obj_set_style_border_width(status_dot, 0, 0);

    connection_label = lv_label_create(screen);
    lv_label_set_text(connection_label, LV_SYMBOL_BLUETOOTH "  蓝牙音箱  ·  已连接");
    lv_obj_set_pos(connection_label, 72, 24);
    lv_obj_set_style_text_font(connection_label, &bluetooth_speaker_font_16, 0);
    lv_obj_set_style_text_color(connection_label, lv_color_hex(0xB5D6E6), 0);

    lv_obj_t * album_card = lv_obj_create(screen);
    lv_obj_set_size(album_card, 240, 240);
    lv_obj_set_pos(album_card, 120, 70);
    lv_obj_set_style_radius(album_card, 24, 0);
    lv_obj_set_style_clip_corner(album_card, true, 0);
    lv_obj_set_style_pad_all(album_card, 0, 0);
    lv_obj_set_style_border_width(album_card, 1, 0);
    lv_obj_set_style_border_color(album_card, lv_color_hex(0x27728E), 0);
    lv_obj_set_style_shadow_width(album_card, 26, 0);
    lv_obj_set_style_shadow_color(album_card, lv_color_hex(0x0A90B8), 0);

    lv_obj_t * cover = lv_image_create(album_card);
    lv_image_set_src(cover, &bluetooth_speaker_album_cover);
    lv_obj_set_size(cover, 240, 240);
    lv_obj_center(cover);

    title_label = lv_label_create(screen);
    lv_obj_set_width(title_label, 400);
    lv_obj_set_pos(title_label, 40, 336);
    lv_obj_set_style_text_align(title_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(title_label, &bluetooth_speaker_font_16, 0);
    lv_obj_set_style_text_color(title_label, lv_color_hex(0xF2FAFF), 0);

    artist_label = lv_label_create(screen);
    lv_obj_set_width(artist_label, 400);
    lv_obj_set_pos(artist_label, 40, 366);
    lv_obj_set_style_text_align(artist_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(artist_label, &bluetooth_speaker_font_16, 0);
    lv_obj_set_style_text_color(artist_label, lv_color_hex(0x89AEC0), 0);

    elapsed_label = lv_label_create(screen);
    lv_obj_set_pos(elapsed_label, 40, 410);
    lv_obj_set_style_text_font(elapsed_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(elapsed_label, lv_color_hex(0xA9C9D8), 0);

    duration_label = lv_label_create(screen);
    lv_obj_set_pos(duration_label, 400, 410);
    lv_obj_set_style_text_font(duration_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(duration_label, lv_color_hex(0xA9C9D8), 0);

    progress_bar = lv_bar_create(screen);
    lv_obj_set_size(progress_bar, 400, 7);
    lv_obj_set_pos(progress_bar, 40, 438);
    lv_obj_set_style_radius(progress_bar, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_bg_color(progress_bar, lv_color_hex(0x1A3B4F), LV_PART_MAIN);
    lv_obj_set_style_radius(progress_bar, LV_RADIUS_CIRCLE, LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(progress_bar, lv_color_hex(0x31D5FF), LV_PART_INDICATOR);

    (void)create_control_button(screen, LV_SYMBOL_PREV, 76, 478, 58, SPEAKER_UI_ACTION_PREVIOUS_TRACK, false);
    playback_button_label = create_control_button(screen, LV_SYMBOL_PAUSE, 190, 468, 100,
                                                   SPEAKER_UI_ACTION_TOGGLE_PLAYBACK, true);
    (void)create_control_button(screen, LV_SYMBOL_NEXT, 346, 478, 58, SPEAKER_UI_ACTION_NEXT_TRACK, false);

    lv_obj_t * volume_label = lv_label_create(screen);
    lv_label_set_text(volume_label, LV_SYMBOL_VOLUME_MID "  音量");
    lv_obj_set_pos(volume_label, 40, 575);
    lv_obj_set_style_text_font(volume_label, &bluetooth_speaker_font_16, 0);
    lv_obj_set_style_text_color(volume_label, lv_color_hex(0xB5D6E6), 0);

    volume_slider = lv_slider_create(screen);
    lv_obj_set_size(volume_slider, 310, 8);
    lv_obj_set_pos(volume_slider, 130, 584);
    lv_slider_set_range(volume_slider, 0, 100);
    lv_obj_set_style_bg_color(volume_slider, lv_color_hex(0x1A3B4F), LV_PART_MAIN);
    lv_obj_set_style_bg_color(volume_slider, lv_color_hex(0x31D5FF), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(volume_slider, lv_color_hex(0xDDF8FF), LV_PART_KNOB);
    lv_obj_set_style_width(volume_slider, 18, LV_PART_KNOB);
    lv_obj_set_style_height(volume_slider, 18, LV_PART_KNOB);
    lv_obj_add_event_cb(volume_slider, volume_slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

void speaker_ui_render(const speaker_view_model_t * view_model)
{
    rendering = true;
    lv_label_set_text(connection_label, view_model->connected ?
                      LV_SYMBOL_BLUETOOTH "  蓝牙音箱  ·  已连接" : "蓝牙音箱  ·  未连接");
    lv_label_set_text(title_label, view_model->track_title);
    lv_label_set_text(artist_label, view_model->artist);
    set_time_label(elapsed_label, view_model->position_seconds);
    set_time_label(duration_label, view_model->duration_seconds);
    lv_bar_set_range(progress_bar, 0, view_model->duration_seconds);
    lv_bar_set_value(progress_bar, view_model->position_seconds, LV_ANIM_OFF);
    lv_label_set_text(playback_button_label, view_model->playing ? LV_SYMBOL_PAUSE : LV_SYMBOL_PLAY);
    lv_slider_set_value(volume_slider, view_model->volume_percent, LV_ANIM_OFF);
    rendering = false;
}
