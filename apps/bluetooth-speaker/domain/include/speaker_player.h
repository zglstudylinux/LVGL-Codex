#ifndef BLUETOOTH_SPEAKER_PLAYER_H
#define BLUETOOTH_SPEAKER_PLAYER_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    const char * title;
    const char * artist;
    uint16_t duration_seconds;
} speaker_track_t;

typedef struct {
    bool connected;
    bool playing;
    uint8_t track_index;
    uint8_t volume_percent;
    uint16_t position_seconds;
} speaker_player_t;

void speaker_player_init(speaker_player_t * player);
void speaker_player_toggle_playback(speaker_player_t * player);
void speaker_player_next_track(speaker_player_t * player);
void speaker_player_previous_track(speaker_player_t * player);
void speaker_player_set_volume(speaker_player_t * player, uint8_t volume_percent);
void speaker_player_advance_seconds(speaker_player_t * player, uint16_t seconds);
const speaker_track_t * speaker_player_get_current_track(const speaker_player_t * player);

#endif /* BLUETOOTH_SPEAKER_PLAYER_H */
