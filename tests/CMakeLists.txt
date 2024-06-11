add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/googletest googletest)

function(create_test TARGET)
    add_executable(${TARGET})

    target_sources(${TARGET} PRIVATE
        ${PROJECT_SOURCE_DIR}/tests/main.cpp
        ${ARGN}
    )

    target_include_directories(${TARGET} PRIVATE
        ${GTEST_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/tests/mocks
        ${PROJECT_SOURCE_DIR}/src
    )

    target_link_libraries(${TARGET}
        gmock_main
        gtest_main
    )
    
    add_test(${TARGET} ${TARGET})
endfunction()

add_subdirectory(tests)
