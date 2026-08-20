#include "lvgl_codex/port.h"
#include "lvgl_codex/runtime.h"

#include "lvgl.h"

lvgl_codex_result_t lvgl_codex_runtime_run(const lvgl_codex_app_t * app)
{
    if(app == NULL || app->start == NULL) {
        return LVGL_CODEX_RESULT_INVALID_ARGUMENT;
    }

    lv_init();
    if(!lvgl_codex_port_init()) {
        lv_deinit();
        return LVGL_CODEX_RESULT_PORT_INIT_FAILED;
    }

    lvgl_codex_result_t start_result = app->start();
    if(start_result != LVGL_CODEX_RESULT_OK) {
        lvgl_codex_port_deinit();
        lv_deinit();
        return start_result;
    }
    while(lvgl_codex_port_is_running()) {
        lv_timer_handler();
        lvgl_codex_port_delay(5U);
    }

    if(app->stop != NULL) {
        app->stop();
    }
    lvgl_codex_port_deinit();
    lv_deinit();
    return 0;
}
