#ifndef __APP_IDENTIFIER___APP_STATE_H
#define __APP_IDENTIFIER___APP_STATE_H

#include <stdbool.h>

typedef struct {
    bool enabled;
} app_state_t;

void app_state_init(app_state_t * state);
void app_state_toggle(app_state_t * state);

#endif /* __APP_IDENTIFIER___APP_STATE_H */
