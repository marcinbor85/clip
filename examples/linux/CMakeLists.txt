set(TARGET example)

add_executable(${TARGET})

target_link_libraries(${TARGET} clip)

target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(${TARGET} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/adc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/exit.c
)
