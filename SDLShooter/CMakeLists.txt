cmake_minimum_required(VERSION 3.20)

project(SDLShooter VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

## output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

## file name for the executable
set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(${TARGET}
    main.cpp
)

target_link_libraries(${TARGET} ${SDL2_LIBRARIES} SDL2_image::SDL2_image)