#include "speaker_controller.h"

#include <assert.h>
#include <string.h>

int main(void)
{
    speaker_controller_t controller;
    speaker_controller_init(&controller);

    speaker_view_model_t view_model = speaker_controller_get_view_model(&controller);
    assert(view_model.connected);
    assert(view_model.playing);
    assert(view_model.volume_percent == 68U);
    assert(strcmp(view_model.track_title, "夜色蓝调") == 0);

    const speaker_ui_action_t pause = { .type = SPEAKER_UI_ACTION_TOGGLE_PLAYBACK, .value = 0U };
    speaker_controller_handle_action(&controller, &pause);
    assert(!speaker_controller_get_view_model(&controller).playing);
    speaker_controller_tick(&controller);
    assert(speaker_controller_get_view_model(&controller).position_seconds == 72U);

    speaker_controller_handle_action(&controller, &pause);
    speaker_controller_tick(&controller);
    assert(speaker_controller_get_view_model(&controller).position_seconds == 73U);

    const speaker_ui_action_t next = { .type = SPEAKER_UI_ACTION_NEXT_TRACK, .value = 0U };
    speaker_controller_handle_action(&controller, &next);
    view_model = speaker_controller_get_view_model(&controller);
    assert(view_model.position_seconds == 0U);
    assert(strcmp(view_model.track_title, "海风回响") == 0);

    const speaker_ui_action_t previous = { .type = SPEAKER_UI_ACTION_PREVIOUS_TRACK, .value = 0U };
    speaker_controller_handle_action(&controller, &previous);
    assert(strcmp(speaker_controller_get_view_model(&controller).track_title, "夜色蓝调") == 0);

    speaker_controller_handle_action(&controller, &next);
    const speaker_ui_action_t volume = { .type = SPEAKER_UI_ACTION_SET_VOLUME, .value = 255U };
    speaker_controller_handle_action(&controller, &volume);
    assert(speaker_controller_get_view_model(&controller).volume_percent == 100U);

    for(uint16_t second = 0U; second < 218U; second++) {
        speaker_controller_tick(&controller);
    }
    assert(strcmp(speaker_controller_get_view_model(&controller).track_title, "星际漫游") == 0);
    assert(speaker_controller_get_view_model(&controller).position_seconds == 0U);
    return 0;
}
