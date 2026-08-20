cmake_minimum_required(VERSION 3.21)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_LIST_DIR)
    project(__APP_IDENTIFIER__ LANGUAGES C CXX)
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_EXTENSIONS OFF)
    include(CTest)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/app_manifest.cmake")
get_property(_framework_configured GLOBAL PROPERTY LVGL_CODEX_FRAMEWORK_CONFIGURED)
if(NOT _framework_configured)
    include(FetchContent)
    include("${CMAKE_CURRENT_LIST_DIR}/framework_lock.cmake")
    set(LVGL_TARGET "pc-sdl2" CACHE STRING "Target port")
    set(LVGL_SERIES "${LVGL_CODEX_APP_LVGL_SERIES}" CACHE STRING "LVGL series")
    set(LVGL_CODEX_FRAMEWORK_SOURCE "" CACHE PATH "Optional local LVGL-Codex framework source")

    if(LVGL_CODEX_FRAMEWORK_SOURCE)
        add_subdirectory("${LVGL_CODEX_FRAMEWORK_SOURCE}" "${CMAKE_BINARY_DIR}/lvgl-codex-framework")
        set(_framework_dir "${LVGL_CODEX_FRAMEWORK_SOURCE}")
    else()
        FetchContent_Declare(lvgl_codex
            GIT_REPOSITORY "${LVGL_CODEX_FRAMEWORK_REPOSITORY}"
            GIT_TAG "${LVGL_CODEX_FRAMEWORK_TAG}"
            GIT_SHALLOW TRUE)
        FetchContent_MakeAvailable(lvgl_codex)
        set(_framework_dir "${lvgl_codex_SOURCE_DIR}")
    endif()
    include("${_framework_dir}/cmake/LvglCodexFramework.cmake")
    if(NOT LVGL_CODEX_LVGL_SERIES STREQUAL LVGL_CODEX_APP_LVGL_SERIES)
        message(FATAL_ERROR "framework_lock.cmake does not match this application's LVGL series.")
    endif()
    if(LVGL_CODEX_LVGL_SERIES STREQUAL "9" AND
       NOT LVGL_CODEX_LVGL_VERSION STREQUAL LVGL_CODEX_LVGL_V9_VERSION)
        message(FATAL_ERROR "framework_lock.cmake does not match the framework's locked LVGL v9 version.")
    endif()
    lvgl_codex_configure_framework(APP_DIR "${CMAKE_CURRENT_LIST_DIR}" TARGET "${LVGL_TARGET}" SERIES "${LVGL_SERIES}")
endif()

lvgl_codex_add_app(
    ID __APP_IDENTIFIER__
    APP_DIR "${CMAKE_CURRENT_LIST_DIR}"
    ENTRY_SOURCE "${CMAKE_CURRENT_LIST_DIR}/src/app_entry.c"
    DOMAIN_SOURCES "${CMAKE_CURRENT_LIST_DIR}/domain/src/app_state.c"
    APPLICATION_SOURCES "${CMAKE_CURRENT_LIST_DIR}/application/src/app_controller.c"
    UI_SOURCES "${CMAKE_CURRENT_LIST_DIR}/ui/src/app_ui.c"
)

lvgl_codex_add_logic_test(
    NAME __APP_IDENTIFIER___logic_tests
    SOURCES "${CMAKE_CURRENT_LIST_DIR}/tests/test_app_controller.c"
    INCLUDE_DIRS
        "${CMAKE_CURRENT_LIST_DIR}/domain/include"
        "${CMAKE_CURRENT_LIST_DIR}/application/include"
        "${CMAKE_CURRENT_LIST_DIR}/ui/include"
    LIBRARIES __APP_IDENTIFIER___domain __APP_IDENTIFIER___application
)
