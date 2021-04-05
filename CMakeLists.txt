cmake_minimum_required(VERSION 3.8)

project(CapitalWorld VERSION 0.1 DESCRIPTION "Capital World is a 2d isometric economy simulation game.")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if(MINGW)
    set(SDL2_LIBRARY sdl2)
    set(SDL2MAIN_LIBRARY SDL2main)
    set(SDL2_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2)
    set(SDL2_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2/include)

    set(SDL2_IMAGE_LIBRARY sdl2_image)
    set(SDL2_IMAGE_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2)
    set(SDL2_IMAGE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2/include)

    set(SDL2TTF_LIBRARY sdl2ttf)
    set(SDL2TTF_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2)
    set(SDL2TTF_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2/include)

    set(SDL2_MIXER_LIBRARY sdl2ttf)
    set(SDL2_MIXER_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2)
    set(SDL2_MIXER_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/SDL2/include)

    set(Intl_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/gettext/include )

    #message("${PROJECT_SOURCE_DIR}/deps/gettext/lib/")

    link_directories(${SDL2_DIR}/lib/ ${PROJECT_SOURCE_DIR}/deps/gettext/lib/ )
elseif (WIN32)
    set(SDL2_LIBRARY sdl2)
    set(SDL2MAIN_LIBRARY SDL2main)
    set(SDL2_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2)
    set(SDL2_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2/include)

    set(SDL2_IMAGE_LIBRARY sdl2_image)
    set(SDL2_IMAGE_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2)
    set(SDL2_IMAGE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2/include)

    set(SDL2TTF_LIBRARY sdl2ttf)
    set(SDL2TTF_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2)
    set(SDL2TTF_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2/include)

    set(SDL2_MIXER_LIBRARY sdl2ttf)
    set(SDL2_MIXER_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2)
    set(SDL2_MIXER_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2/include)

    link_directories(${SDL2_DIR}/lib/x64)
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_gfx)
find_package(SDL2TTF REQUIRED)
find_package(ZLIB)

INCLUDE_DIRECTORIES()
file(GLOB_RECURSE SRC
   # engine/*
    src/*.cpp)

set (GAME_VERSION_MAJOR 0)
set (GAME_VERSION_MINOR 1)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

#   set (MAGIC_ENUM_ROOT "deps")
#   set (MAGIC_ENUM_SRC "${PROJECT_BINARY_DIR}/${MAGIC_ENUM_ROOT}/magic_enum/include" )
  
#   include(magic_enum)
#   message(${MAGIC_ENUM_SRC})

  set (ENGINE_ROOT "deps")
  set (ENGINE_SRC "${PROJECT_BINARY_DIR}/engine/engine-src" )

  message(${ENGINE_SRC})
option(ENGINE_TESTS off)
include(engine)
fetch_engine(
${PROJECT_SOURCE_DIR}/cmake
${PROJECT_BINARY_DIR}/engine
)
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}" ${SDL2_INCLUDE_DIR} ${GETTEXT_INCLUDE_DIR} ${Intl_INCLUDE_DIR} ${MAGIC_ENUM_SRC} ${ENGINE_SRC})


add_executable(${PROJECT_NAME} "src/main.cpp" ${SRC} )
target_link_libraries(${PROJECT_NAME} sgl)
if(MINGW)
    IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
        message("debug mode")
        target_link_libraries(${PROJECT_NAME} SDL2)
    else()
        target_link_libraries(${PROJECT_NAME} mingw32 SDL2main SDL2 -mwindows)
    ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)
elsif(WIN32)
    target_link_libraries(${PROJECT_NAME} SDL2main SDL2 -mwindows)
else()
    target_link_libraries(${PROJECT_NAME} SDL2)
    message("unix "+${SDL2_LIBRARIES})
endif()
target_link_libraries(${PROJECT_NAME} SDL2_ttf)
target_link_libraries(${PROJECT_NAME} SDL2_image)
target_link_libraries(${PROJECT_NAME} SDL2_mixer)
if( UNIX)
  target_link_libraries(${PROJECT_NAME} SDL2_gfx)
  target_link_libraries(${PROJECT_NAME} pthread)
endif()


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

