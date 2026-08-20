# 平台移植

## PC SDL2

使用 `LVGL_TARGET=pc-sdl2`，配置文件为 `apps/<app>/config/pc-sdl2/lv_conf.h`。这是所有 UI 的首选开发与手动验收目标。

应用的 `app_manifest.cmake` 可选声明 `LVGL_CODEX_APP_PC_SDL2_WIDTH`、`LVGL_CODEX_APP_PC_SDL2_HEIGHT` 和 `LVGL_CODEX_APP_PC_SDL2_TITLE`。未声明时兼容旧应用的 800×480 默认值；竖屏应用应显式声明自己的尺寸。

## Linux fbdev + evdev

使用 `LVGL_TARGET=linux-fbdev-evdev`，配置文件为 `apps/<app>/config/linux-fbdev-evdev/lv_conf.h`。运行时通过 `LVGL_CODEX_FBDEV` 指定帧缓冲设备，通过 `LVGL_CODEX_EVDEV` 指定触摸/鼠标设备；未设置时默认 `/dev/fb0` 且不创建输入设备。

## MCU

新建 `boards/<vendor>-<board>/`，实现 `framework/ports/include/lvgl_codex/port.h`，并使用 `ports/mcu/contract` 描述 LCD flush、触摸读取和 tick。为该板独立创建 `lv_conf.h`、工具链文件和 CMake target；不要修改应用的 domain、application 或 ui 契约。
