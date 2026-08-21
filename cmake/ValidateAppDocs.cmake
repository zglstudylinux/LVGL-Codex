cmake_minimum_required(VERSION 3.21)

if(NOT DEFINED LVGL_CODEX_ROOT)
    get_filename_component(LVGL_CODEX_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
endif()

set(_required_sections
    "## 1. 应用目标与边界"
    "## 2. 快速运行"
    "## 3. 总体架构"
    "## 4. 代码架构"
    "## 5. 运行与交互流程"
    "## 6. 代码地图"
    "## 7. 分层原理"
    "## 8. 新功能开发流程"
    "## 9. 平台移植、资源与排错")

set(_validation_failed FALSE)

function(lvgl_codex_validate_app_readme readme expected_title allow_placeholders)
    if(NOT EXISTS "${readme}")
        message(SEND_ERROR "Missing application documentation: ${readme}")
        set(_validation_failed TRUE PARENT_SCOPE)
        return()
    endif()

    file(READ "${readme}" _content)
    string(REGEX MATCH "^# ${expected_title}([ \t]*[\r\n])" _title_match "${_content}")
    if(NOT _title_match)
        message(SEND_ERROR "${readme}: expected first-level title '# ${expected_title}'.")
        set(_validation_failed TRUE PARENT_SCOPE)
    endif()

    foreach(_section IN LISTS _required_sections)
        string(FIND "${_content}" "${_section}" _section_index)
        if(_section_index EQUAL -1)
            message(SEND_ERROR "${readme}: missing required section '${_section}'.")
            set(_validation_failed TRUE PARENT_SCOPE)
        endif()
    endforeach()

    string(REGEX MATCHALL "```mermaid" _mermaid_blocks "${_content}")
    list(LENGTH _mermaid_blocks _mermaid_count)
    if(_mermaid_count LESS 3)
        message(SEND_ERROR "${readme}: expected at least three Mermaid diagrams; found ${_mermaid_count}.")
        set(_validation_failed TRUE PARENT_SCOPE)
    endif()

    string(FIND "${_content}" "sequenceDiagram" _sequence_diagram_index)
    if(_sequence_diagram_index EQUAL -1)
        message(SEND_ERROR "${readme}: missing Mermaid sequenceDiagram interaction flow.")
        set(_validation_failed TRUE PARENT_SCOPE)
    endif()

    if(NOT allow_placeholders)
        string(FIND "${_content}" "__APP_" _placeholder_index)
        if(NOT _placeholder_index EQUAL -1)
            message(SEND_ERROR "${readme}: unresolved application placeholder found.")
            set(_validation_failed TRUE PARENT_SCOPE)
        endif()
    endif()
endfunction()

lvgl_codex_validate_app_readme(
    "${LVGL_CODEX_ROOT}/templates/app/README.md"
    "__APP_NAME__"
    TRUE)

file(GLOB _app_directories LIST_DIRECTORIES TRUE "${LVGL_CODEX_ROOT}/apps/*")
foreach(_app_directory IN LISTS _app_directories)
    if(IS_DIRECTORY "${_app_directory}" AND EXISTS "${_app_directory}/app_manifest.cmake")
        get_filename_component(_app_id "${_app_directory}" NAME)
        lvgl_codex_validate_app_readme("${_app_directory}/README.md" "${_app_id}" FALSE)
    endif()
endforeach()

if(_validation_failed)
    message(FATAL_ERROR "Application documentation validation failed.")
endif()

message(STATUS "Application documentation validation passed.")
