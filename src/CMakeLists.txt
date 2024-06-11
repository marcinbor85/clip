set(TARGET clip)

add_library(${TARGET} STATIC)

target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${TARGET} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/clip_cmd_parse.c
    ${CMAKE_CURRENT_SOURCE_DIR}/clip_cmd_call.c
    ${CMAKE_CURRENT_SOURCE_DIR}/clip_notify.c
    ${CMAKE_CURRENT_SOURCE_DIR}/clip_utils_arg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/clip_utils_hex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/clip_utils_parse.c
)
