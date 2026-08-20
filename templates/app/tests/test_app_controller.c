#include "app_controller.h"

#include <assert.h>

int main(void)
{
    app_controller_t controller;
    app_controller_init(&controller);
    assert(!app_controller_get_view_model(&controller).enabled);
    app_controller_handle_action(&controller, APP_UI_ACTION_TOGGLE);
    assert(app_controller_get_view_model(&controller).enabled);
    return 0;
}
