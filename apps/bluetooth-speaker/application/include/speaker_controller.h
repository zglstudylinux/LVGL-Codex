#ifndef BLUETOOTH_SPEAKER_CONTROLLER_H
#define BLUETOOTH_SPEAKER_CONTROLLER_H

#include "speaker_player.h"
#include "speaker_ui.h"

typedef struct {
    speaker_player_t player;
} speaker_controller_t;

void speaker_controller_init(speaker_controller_t * controller);
void speaker_controller_handle_action(speaker_controller_t * controller, const speaker_ui_action_t * action);
void speaker_controller_tick(speaker_controller_t * controller);
speaker_view_model_t speaker_controller_get_view_model(const speaker_controller_t * controller);

#endif /* BLUETOOTH_SPEAKER_CONTROLLER_H */
