cmake_minimum_required(VERSION 2.8)

project(CapitalWorld)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
find_package(sdl2 REQUIRED)
find_package(SDL2_image)
find_package(SDL2_mixer)
find_package(SDL2_gfx)
find_package(SDL2TTF)
find_package(ZLIB)
file(GLOB_RECURSE SRC . *.cpp)


add_executable(${PROJECT_NAME} "main.cpp" ${SRC})

target_link_libraries(${PROJECT_NAME} SDL2)
target_link_libraries(${PROJECT_NAME} SDL2_ttf)
target_link_libraries(${PROJECT_NAME} SDL2_image)
target_link_libraries(${PROJECT_NAME} SDL2_mixer)
target_link_libraries(${PROJECT_NAME} SDL2_gfx)
target_link_libraries(${PROJECT_NAME} z)
add_custom_command(TARGET CapitalWorld PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/fonts/ $<TARGET_FILE_DIR:CapitalWorld>/fonts/)
add_custom_command(TARGET CapitalWorld PRE_BUILD
                  COMMAND ${CMAKE_COMMAND} -E copy_directory
                      ${CMAKE_SOURCE_DIR}/images/ $<TARGET_FILE_DIR:CapitalWorld>/images/)
