cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project(sdltest)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
#find_package(SDL2_image REQUIRED)
#find_package(SDL2_ttf REQUIRED)
set (SDL2_TTF_INCLUDE_DIR "/usr/local/Cellar/sdl2_ttf/2.0.15/include/SDL2")
set (SDL_TTF_INCLUDE_DIR "/usr/local/Cellar/sdl_ttf/2.0.11_1/include/SDL")

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIR} src)
#include_directories(${SDL2_INCLUDE_DIRS} ${SDL_IMAGE_INCLUDE_DIR} ${SDL_TTF_INCLUDE_DIR} src)

add_executable(sdltest src/main.cpp)
set (SDL2_TTF_LIBRARIES "/usr/local/Cellar/sdl2_ttf/2.0.15")
set (SDL_TTF_LIBRARIES "/usr/local/Cellar/sdl_ttf/2.0.11_1/lib")
#target_link_libraries(sdltest ${SDL2_LIBRARIES} ${SDL_TTF_LIBRARIES} ${SDL_IMAGE_LIBRARIES})
target_link_libraries(sdltest ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})