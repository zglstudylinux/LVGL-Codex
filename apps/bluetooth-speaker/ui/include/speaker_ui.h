#ifndef BLUETOOTH_SPEAKER_UI_H
#define BLUETOOTH_SPEAKER_UI_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    SPEAKER_UI_ACTION_PREVIOUS_TRACK,
    SPEAKER_UI_ACTION_TOGGLE_PLAYBACK,
    SPEAKER_UI_ACTION_NEXT_TRACK,
    SPEAKER_UI_ACTION_SET_VOLUME,
} speaker_ui_action_type_t;

typedef struct {
    speaker_ui_action_type_t type;
    uint8_t value;
} speaker_ui_action_t;

typedef void (*speaker_ui_action_handler_t)(const speaker_ui_action_t * action, void * context);

typedef struct {
    bool connected;
    bool playing;
    const char * track_title;
    const char * artist;
    uint16_t position_seconds;
    uint16_t duration_seconds;
    uint8_t volume_percent;
} speaker_view_model_t;

void speaker_ui_create(speaker_ui_action_handler_t action_handler, void * context);
void speaker_ui_render(const speaker_view_model_t * view_model);

#endif /* BLUETOOTH_SPEAKER_UI_H */
