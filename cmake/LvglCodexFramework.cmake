include_guard(GLOBAL)

include(CMakeParseArguments)
include(FetchContent)
set(LVGL_CODEX_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." CACHE INTERNAL "LVGL-Codex root")
include("${LVGL_CODEX_ROOT}/cmake/dependencies/lvgl_versions.cmake")

function(lvgl_codex_configure_framework)
    cmake_parse_arguments(ARG "" "APP_DIR;TARGET;SERIES" "" ${ARGN})
    if(NOT ARG_APP_DIR OR NOT ARG_TARGET OR NOT ARG_SERIES)
        message(FATAL_ERROR "lvgl_codex_configure_framework requires APP_DIR, TARGET and SERIES.")
    endif()

    get_property(_configured GLOBAL PROPERTY LVGL_CODEX_FRAMEWORK_CONFIGURED)
    if(_configured)
        return()
    endif()

    set(_config_dir "${ARG_APP_DIR}/config/${ARG_TARGET}")
    if(NOT EXISTS "${_config_dir}/lv_conf.h")
        message(FATAL_ERROR "Missing target LVGL configuration: ${_config_dir}/lv_conf.h")
    endif()

    lvgl_codex_resolve_lvgl("${ARG_SERIES}" _lvgl_version _lvgl_url _lvgl_sha256)
    message(STATUS "LVGL-Codex: ${ARG_TARGET}, LVGL v${_lvgl_version}")

    set(CONFIG_LV_BUILD_EXAMPLES OFF CACHE BOOL "Build LVGL examples" FORCE)
    set(CONFIG_LV_BUILD_DEMOS OFF CACHE BOOL "Build LVGL demos" FORCE)
    set(CONFIG_LV_USE_THORVG_INTERNAL OFF CACHE BOOL "Build LVGL internal ThorVG" FORCE)
    set(LV_BUILD_CONF_DIR "${_config_dir}" CACHE PATH "LVGL configuration directory" FORCE)

    if(ARG_TARGET STREQUAL "pc-sdl2")
        find_package(SDL2 CONFIG REQUIRED)
    elseif(ARG_TARGET STREQUAL "linux-fbdev-evdev")
        if(NOT UNIX OR APPLE)
            message(FATAL_ERROR "linux-fbdev-evdev must be configured on a Linux host or Linux cross toolchain.")
        endif()
    else()
        message(FATAL_ERROR "Unknown LVGL target '${ARG_TARGET}'. Add a port and board package first.")
    endif()

    FetchContent_Declare(
        lvgl
        URL "${_lvgl_url}"
        URL_HASH "SHA256=${_lvgl_sha256}"
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(lvgl)

    if(ARG_TARGET STREQUAL "pc-sdl2")
        target_link_libraries(lvgl PUBLIC SDL2::SDL2)
    endif()

    add_library(lvgl_codex_compat INTERFACE)
    if(ARG_SERIES STREQUAL "9")
        target_include_directories(lvgl_codex_compat INTERFACE
            "${LVGL_CODEX_ROOT}/framework/lvgl_compat/v9/include")
    elseif(ARG_SERIES STREQUAL "8")
        target_include_directories(lvgl_codex_compat INTERFACE
            "${LVGL_CODEX_ROOT}/framework/lvgl_compat/v8/include")
    endif()
    target_link_libraries(lvgl_codex_compat INTERFACE lvgl)

    add_library(lvgl_codex_contracts INTERFACE)
    target_include_directories(lvgl_codex_contracts
        INTERFACE "${LVGL_CODEX_ROOT}/framework/contracts/include")

    add_library(lvgl_codex_runtime STATIC
        "${LVGL_CODEX_ROOT}/framework/runtime/src/lvgl_runtime.c"
        "${LVGL_CODEX_ROOT}/framework/runtime/src/main.c"
    )
    target_include_directories(lvgl_codex_runtime
        PUBLIC
            "${LVGL_CODEX_ROOT}/framework/contracts/include"
            "${LVGL_CODEX_ROOT}/framework/ports/include"
    )
    target_link_libraries(lvgl_codex_runtime PUBLIC
        lvgl_codex_contracts
        lvgl_codex_compat)

    if(ARG_TARGET STREQUAL "pc-sdl2")
        add_library(lvgl_codex_port STATIC
            "${LVGL_CODEX_ROOT}/ports/pc/sdl2/src/port_sdl2.c")
        target_include_directories(lvgl_codex_port PRIVATE
            "${LVGL_CODEX_ROOT}/framework/ports/include")
        target_link_libraries(lvgl_codex_port PUBLIC lvgl_codex_compat SDL2::SDL2)
    elseif(ARG_TARGET STREQUAL "linux-fbdev-evdev")
        add_library(lvgl_codex_port STATIC
            "${LVGL_CODEX_ROOT}/ports/linux/fbdev_evdev/src/port_linux_fbdev_evdev.c")
        target_include_directories(lvgl_codex_port PRIVATE
            "${LVGL_CODEX_ROOT}/framework/ports/include")
        target_link_libraries(lvgl_codex_port PUBLIC lvgl_codex_compat)
    endif()

    target_link_libraries(lvgl_codex_runtime PRIVATE lvgl_codex_port)
    set_property(GLOBAL PROPERTY LVGL_CODEX_FRAMEWORK_CONFIGURED TRUE)
endfunction()

function(lvgl_codex_add_app)
    cmake_parse_arguments(ARG "" "ID;APP_DIR;ENTRY_SOURCE" "DOMAIN_SOURCES;APPLICATION_SOURCES;UI_SOURCES" ${ARGN})
    if(NOT ARG_ID OR NOT ARG_APP_DIR OR NOT ARG_ENTRY_SOURCE)
        message(FATAL_ERROR "lvgl_codex_add_app requires ID, APP_DIR and ENTRY_SOURCE.")
    endif()

    add_library(${ARG_ID}_domain STATIC ${ARG_DOMAIN_SOURCES})
    target_include_directories(${ARG_ID}_domain PUBLIC "${ARG_APP_DIR}/domain/include")
    target_link_libraries(${ARG_ID}_domain PUBLIC lvgl_codex_contracts)

    add_library(${ARG_ID}_application STATIC ${ARG_APPLICATION_SOURCES})
    target_include_directories(${ARG_ID}_application
        PUBLIC
            "${ARG_APP_DIR}/application/include"
            "${ARG_APP_DIR}/ui/include"
        PRIVATE "${ARG_APP_DIR}/domain/include")
    target_link_libraries(${ARG_ID}_application PUBLIC ${ARG_ID}_domain lvgl_codex_contracts)

    add_library(${ARG_ID}_ui STATIC ${ARG_UI_SOURCES})
    target_include_directories(${ARG_ID}_ui PUBLIC "${ARG_APP_DIR}/ui/include")
    target_link_libraries(${ARG_ID}_ui PUBLIC lvgl_codex_compat)

    add_executable(${ARG_ID} ${ARG_ENTRY_SOURCE})
    target_include_directories(${ARG_ID} PRIVATE
        "${ARG_APP_DIR}/application/include"
        "${ARG_APP_DIR}/ui/include")
    target_link_libraries(${ARG_ID} PRIVATE
        lvgl_codex_runtime
        ${ARG_ID}_application
        ${ARG_ID}_ui)
endfunction()

function(lvgl_codex_add_logic_test)
    cmake_parse_arguments(ARG "" "NAME" "SOURCES;INCLUDE_DIRS;LIBRARIES" ${ARGN})
    if(NOT BUILD_TESTING)
        return()
    endif()
    add_executable(${ARG_NAME} ${ARG_SOURCES})
    target_include_directories(${ARG_NAME} PRIVATE ${ARG_INCLUDE_DIRS})
    target_link_libraries(${ARG_NAME} PRIVATE ${ARG_LIBRARIES})
    add_test(NAME ${ARG_NAME} COMMAND ${ARG_NAME})
endfunction()
