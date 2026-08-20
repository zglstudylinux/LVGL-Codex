cmake_minimum_required(VERSION 3.21)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_LIST_DIR)
    project(LVGL_Codex VERSION 0.2.0 LANGUAGES C CXX)

    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_EXTENSIONS OFF)

    include(CTest)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/cmake/LvglCodexFramework.cmake")

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_LIST_DIR)
    set(LVGL_APP "device-dashboard" CACHE STRING "Application directory name under apps/")
    set(LVGL_TARGET "pc-sdl2" CACHE STRING "Target port: pc-sdl2 or linux-fbdev-evdev")
    set(LVGL_SERIES "9" CACHE STRING "LVGL major version series")
    set_property(CACHE LVGL_TARGET PROPERTY STRINGS pc-sdl2 linux-fbdev-evdev)
    set_property(CACHE LVGL_SERIES PROPERTY STRINGS 8 9)

    set(_app_dir "${CMAKE_CURRENT_SOURCE_DIR}/apps/${LVGL_APP}")
    if(NOT EXISTS "${_app_dir}/app_manifest.cmake")
        message(FATAL_ERROR "Unknown LVGL_APP='${LVGL_APP}'. Expected apps/<app-id>/app_manifest.cmake.")
    endif()

    include("${_app_dir}/app_manifest.cmake")
    if(NOT LVGL_SERIES STREQUAL LVGL_CODEX_APP_LVGL_SERIES)
        message(FATAL_ERROR
            "Application '${LVGL_CODEX_APP_ID}' declares LVGL series ${LVGL_CODEX_APP_LVGL_SERIES}; "
            "requested ${LVGL_SERIES}.")
    endif()
    list(FIND LVGL_CODEX_APP_SUPPORTED_TARGETS "${LVGL_TARGET}" _target_index)
    if(_target_index EQUAL -1)
        message(FATAL_ERROR
            "Application '${LVGL_CODEX_APP_ID}' does not support LVGL_TARGET='${LVGL_TARGET}'. "
            "Supported targets: ${LVGL_CODEX_APP_SUPPORTED_TARGETS}")
    endif()

    lvgl_codex_configure_framework(
        APP_DIR "${_app_dir}"
        TARGET "${LVGL_TARGET}"
        SERIES "${LVGL_SERIES}"
    )
    add_subdirectory("${_app_dir}")
endif()
