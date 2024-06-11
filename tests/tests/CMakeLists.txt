create_test(test_clip_utils_arg
    ${CMAKE_CURRENT_SOURCE_DIR}/test_clip_utils_arg.cpp
    ${PROJECT_SOURCE_DIR}/src/clip_utils_arg.c
)

create_test(test_clip_utils_parse
    ${CMAKE_CURRENT_SOURCE_DIR}/test_clip_utils_parse.cpp
    ${PROJECT_SOURCE_DIR}/src/clip_utils_parse.c
)

create_test(test_clip_utils_hex
    ${CMAKE_CURRENT_SOURCE_DIR}/test_clip_utils_hex.cpp
    ${PROJECT_SOURCE_DIR}/src/clip_utils_hex.c
)

create_test(test_clip_notify
    ${CMAKE_CURRENT_SOURCE_DIR}/test_clip_notify.cpp
    ${PROJECT_SOURCE_DIR}/src/clip_notify.c
)

create_test(test_clip_cmd_parse
    ${CMAKE_CURRENT_SOURCE_DIR}/test_clip_cmd_parse.cpp
    ${PROJECT_SOURCE_DIR}/src/clip_cmd_parse.c
)

create_test(test_clip_cmd_call
    ${CMAKE_CURRENT_SOURCE_DIR}/test_clip_cmd_call.cpp
    ${PROJECT_SOURCE_DIR}/src/clip_cmd_call.c
)

create_test(test_clip_e2e
    ${CMAKE_CURRENT_SOURCE_DIR}/test_clip_e2e.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_clip_e2e_tree.c
)
target_link_libraries(test_clip_e2e clip)