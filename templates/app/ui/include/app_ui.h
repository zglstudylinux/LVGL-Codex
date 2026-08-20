#ifndef __APP_IDENTIFIER___APP_UI_H
#define __APP_IDENTIFIER___APP_UI_H

#include <stdbool.h>

typedef enum {
    APP_UI_ACTION_TOGGLE,
} app_ui_action_t;

typedef void (*app_ui_action_handler_t)(app_ui_action_t action, void * context);

typedef struct {
    bool enabled;
} app_view_model_t;

void app_ui_create(app_ui_action_handler_t handler, void * context);
void app_ui_render(const app_view_model_t * view_model);

#endif /* __APP_IDENTIFIER___APP_UI_H */
