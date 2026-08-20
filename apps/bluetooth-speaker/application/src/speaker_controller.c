#include "speaker_controller.h"

void speaker_controller_init(speaker_controller_t * controller)
{
    speaker_player_init(&controller->player);
}

void speaker_controller_handle_action(speaker_controller_t * controller, const speaker_ui_action_t * action)
{
    switch(action->type) {
        case SPEAKER_UI_ACTION_PREVIOUS_TRACK:
            speaker_player_previous_track(&controller->player);
            break;
        case SPEAKER_UI_ACTION_TOGGLE_PLAYBACK:
            speaker_player_toggle_playback(&controller->player);
            break;
        case SPEAKER_UI_ACTION_NEXT_TRACK:
            speaker_player_next_track(&controller->player);
            break;
        case SPEAKER_UI_ACTION_SET_VOLUME:
            speaker_player_set_volume(&controller->player, action->value);
            break;
        default:
            break;
    }
}

void speaker_controller_tick(speaker_controller_t * controller)
{
    speaker_player_advance_seconds(&controller->player, 1U);
}

speaker_view_model_t speaker_controller_get_view_model(const speaker_controller_t * controller)
{
    const speaker_track_t * track = speaker_player_get_current_track(&controller->player);
    speaker_view_model_t view_model = {
        .connected = controller->player.connected,
        .playing = controller->player.playing,
        .track_title = track->title,
        .artist = track->artist,
        .position_seconds = controller->player.position_seconds,
        .duration_seconds = track->duration_seconds,
        .volume_percent = controller->player.volume_percent,
    };
    return view_model;
}
