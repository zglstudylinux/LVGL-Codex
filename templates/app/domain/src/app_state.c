#include "app_state.h"

void app_state_init(app_state_t * state)
{
    state->enabled = false;
}

void app_state_toggle(app_state_t * state)
{
    state->enabled = !state->enabled;
}
