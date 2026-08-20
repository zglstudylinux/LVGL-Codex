#include "lvgl_codex/port.h"

#include "lvgl_compat.h"

#include "src/drivers/sdl/lv_sdl_window.h"

#ifndef LVGL_CODEX_PC_SDL2_WIDTH
#define LVGL_CODEX_PC_SDL2_WIDTH 800
#endif

#ifndef LVGL_CODEX_PC_SDL2_HEIGHT
#define LVGL_CODEX_PC_SDL2_HEIGHT 480
#endif

#ifndef LVGL_CODEX_PC_SDL2_TITLE
#define LVGL_CODEX_PC_SDL2_TITLE "LVGL-Codex"
#endif

static bool running;

bool lvgl_codex_port_init(void)
{
    lv_display_t * display = lv_sdl_window_create(LVGL_CODEX_PC_SDL2_WIDTH, LVGL_CODEX_PC_SDL2_HEIGHT);
    if(display == NULL) {
        return false;
    }

    lv_sdl_window_set_title(display, LVGL_CODEX_PC_SDL2_TITLE);
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
