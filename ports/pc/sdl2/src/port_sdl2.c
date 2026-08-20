#include "lvgl_codex/port.h"

#include "lvgl_compat.h"

#include "src/drivers/sdl/lv_sdl_window.h"

static bool running;

bool lvgl_codex_port_init(void)
{
    lv_display_t * display = lv_sdl_window_create(800, 480);
    if(display == NULL) {
        return false;
    }

    lv_sdl_window_set_title(display, "LVGL-Codex - device-dashboard");
    running = true;
    return true;
}

bool lvgl_codex_port_is_running(void)
{
    return running && lvgl_codex_has_active_display();
}

void lvgl_codex_port_delay(uint32_t milliseconds)
{
    lv_delay_ms(milliseconds);
}

void lvgl_codex_port_deinit(void)
{
    running = false;
}
