#ifndef __APP_IDENTIFIER___APP_CONTROLLER_H
#define __APP_IDENTIFIER___APP_CONTROLLER_H

#include "app_state.h"
#include "app_ui.h"

typedef struct {
    app_state_t state;
} app_controller_t;

void app_controller_init(app_controller_t * controller);
void app_controller_handle_action(app_controller_t * controller, app_ui_action_t action);
app_view_model_t app_controller_get_view_model(const app_controller_t * controller);

#endif /* __APP_IDENTIFIER___APP_CONTROLLER_H */
