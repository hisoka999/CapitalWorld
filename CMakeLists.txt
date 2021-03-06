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

set (GAME_VERSION_MAJOR 0)
set (GAME_VERSION_MINOR 1)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")


add_executable(${PROJECT_NAME} "main.cpp" ${SRC})

target_link_libraries(${PROJECT_NAME} SDL2)
target_link_libraries(${PROJECT_NAME} SDL2_ttf)
target_link_libraries(${PROJECT_NAME} SDL2_image)
target_link_libraries(${PROJECT_NAME} SDL2_mixer)
target_link_libraries(${PROJECT_NAME} SDL2_gfx)
target_link_libraries(${PROJECT_NAME} z)
target_link_libraries(${PROJECT_NAME} pthread)

add_custom_command(TARGET CapitalWorld PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/fonts/ $<TARGET_FILE_DIR:CapitalWorld>/fonts/)
add_custom_command(TARGET CapitalWorld PRE_BUILD
                  COMMAND ${CMAKE_COMMAND} -E copy_directory
                      ${CMAKE_SOURCE_DIR}/images/ $<TARGET_FILE_DIR:CapitalWorld>/images/)

INSTALL(TARGETS CapitalWorld DESTINATION capitalworld)
INSTALL(DIRECTORY fonts DESTINATION capitalworld )
INSTALL(DIRECTORY images DESTINATION capitalworld )
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsdl2-2.0-0 (>= 2.0), libsdl2-ttf-2.0-0 (>= 2.0), libsdl2-image-2.0-0 (>= 2.0), libsdl2-gfx-2.0-0 (>= 2.0), libsdl2-mixer-2.0-0 (>= 2.0)")
set(CPACK_PACKAGE_VERSION "${GAME_VERSION_MAJOR}.${GAME_VERSION_MINOR}")
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Stefan Lüdtke") #required
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Capital World is a 2d isometric economy simulation game.")
set(CPACK_DEBIAN_PACKAGE_SECTION "games")
INCLUDE(CPack)

