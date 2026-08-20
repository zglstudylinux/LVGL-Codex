#ifndef LVGL_CODEX_PORT_H
#define LVGL_CODEX_PORT_H

#include <stdbool.h>
#include <stdint.h>

bool lvgl_codex_port_init(void);
bool lvgl_codex_port_is_running(void);
void lvgl_codex_port_delay(uint32_t milliseconds);
void lvgl_codex_port_deinit(void);

#endif /* LVGL_CODEX_PORT_H */
