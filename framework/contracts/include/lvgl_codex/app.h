#ifndef LVGL_CODEX_APP_H
#define LVGL_CODEX_APP_H

#include "lvgl_codex/result.h"

typedef struct {
    const char * id;
    lvgl_codex_result_t (*start)(void);
    void (*stop)(void);
} lvgl_codex_app_t;

const lvgl_codex_app_t * lvgl_codex_app_descriptor(void);

#endif /* LVGL_CODEX_APP_H */
