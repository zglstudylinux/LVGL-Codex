include_guard(GLOBAL)

set(LVGL_CODEX_LVGL_V9_VERSION "9.5.0" CACHE INTERNAL "Locked LVGL v9 version" FORCE)
set(LVGL_CODEX_LVGL_V9_URL "https://github.com/lvgl/lvgl/archive/refs/tags/v9.5.0.zip" CACHE INTERNAL "Locked LVGL v9 source URL" FORCE)
set(LVGL_CODEX_LVGL_V9_SHA256 "ed25a729864e6be6904cb2e5e0c8566366f4798b694c676b62e10f9b54865697" CACHE INTERNAL "Locked LVGL v9 source SHA256" FORCE)

function(lvgl_codex_resolve_lvgl series out_version out_url out_sha256)
    if(series STREQUAL "9")
        set(${out_version} "${LVGL_CODEX_LVGL_V9_VERSION}" PARENT_SCOPE)
        set(${out_url} "${LVGL_CODEX_LVGL_V9_URL}" PARENT_SCOPE)
        set(${out_sha256} "${LVGL_CODEX_LVGL_V9_SHA256}" PARENT_SCOPE)
        return()
    endif()

    if(series STREQUAL "8")
        message(FATAL_ERROR
            "LVGL v8 is intentionally not enabled yet. Add a pinned v8 release, its "
            "configuration profile, and a CI build before declaring an app as v8-compatible.")
    endif()

    message(FATAL_ERROR "Unsupported LVGL series '${series}'.")
endfunction()
