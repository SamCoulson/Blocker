cmake_minimum_required(VERSION 3.0.0)
project(Blocker VERSION 0.1.0)

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(sdl2-ttf CONFIG REQUIRED)

add_executable(Blocker src/main.cpp)

target_link_libraries(Blocker PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2::SDL2_image SDL2::SDL2_ttf)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
