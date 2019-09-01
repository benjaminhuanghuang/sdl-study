cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project(cpp-sdl-study)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL_IMAGE_INCLUDE_DIR} ${SDL_TTF_INCLUDE_DIR} src)

add_executable(sdltest src/main.cpp)

target_link_libraries(sdltest ${SDL2_LIBRARIES} ${SDL_TTF_LIBRARIES} ${SDL_IMAGE_LIBRARIES})
