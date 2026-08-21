#include "speaker_controller.h"
#include "speaker_ui.h"
#include "lvgl_codex/app.h"

#include <stddef.h>

#include "lvgl.h"

static speaker_controller_t controller;
static lv_timer_t * playback_timer;

static void render_current_state(void)
{
    speaker_view_model_t view_model = speaker_controller_get_view_model(&controller);
    speaker_ui_render(&view_model);
}

static void handle_ui_action(const speaker_ui_action_t * action, void * context)
{
    (void)context;
    speaker_controller_handle_action(&controller, action);
    render_current_state();
}

static void playback_timer_cb(lv_timer_t * timer)
{
    (void)timer;
    speaker_controller_tick(&controller);
    render_current_state();
}

static lvgl_codex_result_t bluetooth_speaker_start(void)
{
    speaker_controller_init(&controller);
    speaker_ui_create(handle_ui_action, NULL);
    render_current_state();
    playback_timer = lv_timer_create(playback_timer_cb, 1000U, NULL);
    return playback_timer == NULL ? LVGL_CODEX_RESULT_APP_START_FAILED : LVGL_CODEX_RESULT_OK;
}

static void bluetooth_speaker_stop(void)
{
    if(playback_timer != NULL) {
        lv_timer_delete(playback_timer);
        playback_timer = NULL;
    }
}

const lvgl_codex_app_t * lvgl_codex_app_descriptor(void)
{
    static const lvgl_codex_app_t app = {
        .id = "bluetooth-speaker",
        .start = bluetooth_speaker_start,
        .stop = bluetooth_speaker_stop,
    };
    return &app;
}
