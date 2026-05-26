cmake_minimum_required(VERSION 3.14)
project(pongy)

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp src/Text.cpp)

get_property(importedTargets DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY IMPORTED_TARGETS)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf-static
)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Werror -Wpedantic -Wextra -std=c++23)

set(CMAKE_CXX_STANDARD 23)