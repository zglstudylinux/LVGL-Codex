#include "lvgl_codex/port.h"

#include "lvgl.h"

#include "src/drivers/display/fb/lv_linux_fbdev.h"
#include "src/drivers/evdev/lv_evdev.h"

#include <stdlib.h>

static bool running;

static const char * env_or_default(const char * name, const char * default_value)
{
    const char * value = getenv(name);
    return value != NULL && value[0] != '\0' ? value : default_value;
}

bool lvgl_codex_port_init(void)
{
    const char * framebuffer = env_or_default("LVGL_CODEX_FBDEV", "/dev/fb0");
    const char * input = getenv("LVGL_CODEX_EVDEV");
    lv_display_t * display = lv_linux_fbdev_create();

    if(display == NULL || lv_linux_fbdev_set_file(display, framebuffer) != LV_RESULT_OK) {
        return false;
    }

    if(input != NULL && input[0] != '\0') {
        if(lv_evdev_create(LV_INDEV_TYPE_POINTER, input) == NULL) {
            return false;
        }
    }

    running = true;
    return true;
}

bool lvgl_codex_port_is_running(void)
{
    return running;
}

void lvgl_codex_port_delay(uint32_t milliseconds)
{
    lv_delay_ms(milliseconds);
}

void lvgl_codex_port_deinit(void)
{
    running = false;
}
