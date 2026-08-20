#ifndef LVGL_CODEX_MCU_PORT_CONTRACT_H
#define LVGL_CODEX_MCU_PORT_CONTRACT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint16_t horizontal_resolution;
    uint16_t vertical_resolution;
    uint8_t color_depth;
} lvgl_codex_mcu_display_config_t;

typedef bool (*lvgl_codex_mcu_flush_cb_t)(const void * pixels, size_t pixel_count);
typedef bool (*lvgl_codex_mcu_touch_read_cb_t)(int16_t * x, int16_t * y, bool * pressed);

/* Board packages implement these callbacks and bind them to the selected LVGL adapter. */
typedef struct {
    lvgl_codex_mcu_display_config_t display;
    lvgl_codex_mcu_flush_cb_t flush;
    lvgl_codex_mcu_touch_read_cb_t touch_read;
    uint32_t (*tick_milliseconds)(void);
} lvgl_codex_mcu_port_contract_t;

#endif /* LVGL_CODEX_MCU_PORT_CONTRACT_H */
