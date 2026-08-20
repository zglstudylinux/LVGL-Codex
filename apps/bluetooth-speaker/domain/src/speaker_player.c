#include "speaker_player.h"

#define SPEAKER_TRACK_COUNT 3U

static const speaker_track_t tracks[SPEAKER_TRACK_COUNT] = {
    { "夜色蓝调", "北岸乐队", 243U },
    { "海风回响", "晨雾", 218U },
    { "星际漫游", "空港", 267U },
};

void speaker_player_init(speaker_player_t * player)
{
    player->connected = true;
    player->playing = true;
    player->track_index = 0U;
    player->volume_percent = 68U;
    player->position_seconds = 72U;
}

void speaker_player_toggle_playback(speaker_player_t * player)
{
    player->playing = !player->playing;
}

void speaker_player_next_track(speaker_player_t * player)
{
    player->track_index = (uint8_t)((player->track_index + 1U) % SPEAKER_TRACK_COUNT);
    player->position_seconds = 0U;
}

void speaker_player_previous_track(speaker_player_t * player)
{
    player->track_index = player->track_index == 0U ?
        (uint8_t)(SPEAKER_TRACK_COUNT - 1U) : (uint8_t)(player->track_index - 1U);
    player->position_seconds = 0U;
}

void speaker_player_set_volume(speaker_player_t * player, uint8_t volume_percent)
{
    player->volume_percent = volume_percent > 100U ? 100U : volume_percent;
}

void speaker_player_advance_seconds(speaker_player_t * player, uint16_t seconds)
{
    const speaker_track_t * track = speaker_player_get_current_track(player);

    if(!player->playing) {
        return;
    }

    if((uint32_t)player->position_seconds + seconds >= track->duration_seconds) {
        speaker_player_next_track(player);
        return;
    }

    player->position_seconds = (uint16_t)(player->position_seconds + seconds);
}

const speaker_track_t * speaker_player_get_current_track(const speaker_player_t * player)
{
    return &tracks[player->track_index];
}
