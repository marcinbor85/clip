cmake_minimum_required(VERSION 3.22)

project(clip LANGUAGES C CXX VERSION 0.1.0)

enable_testing()

set(CMAKE_C_FLAGS "-std=c17 -pedantic -Werror -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable")
set(CMAKE_CXX_FLAGS "-std=c++17 -pedantic -Werror -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable")

add_compile_definitions(CLIP_USE_USER_CONFIG)
include_directories(${PROJECT_SOURCE_DIR}/examples/linux/include)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/examples/linux)
add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
