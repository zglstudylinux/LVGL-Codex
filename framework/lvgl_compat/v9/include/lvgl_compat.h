#ifndef LVGL_CODEX_LVGL_COMPAT_H
#define LVGL_CODEX_LVGL_COMPAT_H

#include "lvgl.h"

static inline lv_obj_t * lvgl_codex_active_screen(void)
{
    return lv_screen_active();
}

static inline bool lvgl_codex_has_active_display(void)
{
    return lv_display_get_default() != NULL;
}

#endif /* LVGL_CODEX_LVGL_COMPAT_H */
