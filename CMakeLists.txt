cmake_minimum_required(VERSION 3.14)
project(pongy)

find_package(SDL3 REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Werror -Wpedantic -Wextra -std=c++23)

set(CMAKE_CXX_STANDARD 23)