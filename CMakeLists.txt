cmake_minimum_required(VERSION 3.14)
project(pongy)
add_executable(${PROJECT_NAME} src/main.cpp)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Werror -Wpedantic -Wextra -std=c++23)

set(CMAKE_CXX_STANDARD 23)