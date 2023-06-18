cmake_minimum_required(VERSION 3.8)

project(CapitalWorld VERSION 0.2 DESCRIPTION "Capital World is a 2d isometric economy simulation game.")

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
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    SET(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES})

    set(SDL2_LIBRARY sdl2)
    set(SDL2MAIN_LIBRARY SDL2main)
    set(SDL2_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2 )
    set(SDL2_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2/include)

    set(SDL2_IMAGE_LIBRARY SDL2_image CACHE FILEPATH "")
    set(SDL2_IMAGE_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2 CACHE PATH "")
    set(SDL2_IMAGE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2/include CACHE PATH "")

    set(SDL2TTF_LIBRARY sdl2ttf CACHE FILEPATH "")
    set(SDL2TTF_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2 CACHE PATH "")
    set(SDL2TTF_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2/include CACHE PATH "")

    set(SDL2_MIXER_LIBRARY SDL2_mixer CACHE FILEPATH "")
    set(SDL2_MIXER_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2 CACHE PATH "")
    set(SDL2_MIXER_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2/include CACHE PATH "")

    #link_directories(${SDL2_DIR}/lib/x64)
    if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    # 64 bits
        link_directories(${SDL2_DIR}/lib/x64)
    elseif("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "Win32")
        # 32 bits
        link_directories(${SDL2_DIR}/lib/x86)
        else()
        message("unknown plattform: ${CMAKE_GENERATOR_PLATFORM}")
    endif()
else()
    message("unknown system: ${CMAKE_CXX_COMPILER_ID}")
endif()
message("SDL2 image dir: ${SDL2_IMAGE_DIR}")

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
set (GAME_VERSION_MINOR 2)
write_file("${PROJECT_BINARY_DIR}/VERSION.TXT" "${GAME_VERSION_MAJOR}.${GAME_VERSION_MINOR}")

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

set(BOX2D_BUILD_UNIT_TESTS OFF)
set(BOX2D_BUILD_TESTBED  OFF)
set (ENGINE_ROOT "deps/libsgl")
set (ENGINE_SRC "deps/libsgl" )
option(ENGINE_TESTS off)
  

set (MAGIC_ENUM_ROOT "deps")
set (MAGIC_ENUM_SRC "${PROJECT_BINARY_DIR}/engine-build/${MAGIC_ENUM_ROOT}/magic_enum/magic_enum-src/include/" )
if ( UNIX)
    if(GAME_TESTS)
        option(INSTALL_GTEST OFF)
        include(googletest)
        include(GoogleTest)
        fetch_googletest(
            ${PROJECT_SOURCE_DIR}/cmake
            ${PROJECT_BINARY_DIR}/googletest
            )
        enable_testing() 
        add_subdirectory(test EXCLUDE_FROM_ALL)


    endif()
endif()



add_subdirectory(
    ${ENGINE_SRC}
    ${PROJECT_BINARY_DIR}/engine-build
    )




if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    
endif()

message( ${MAGIC_ENUM_SRC})
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}" "src/" ${SDL2_INCLUDE_DIR} ${GETTEXT_INCLUDE_DIR} ${Intl_INCLUDE_DIR} ${ENGINE_SRC} ${MAGIC_ENUM_SRC})

# Let's set compiler-specific flags
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    # G++
    add_compile_options(-Wall -Wextra -Werror)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    # MSVC
    target_compile_options(${PROJECT_NAME} PRIVATE /EHsc /MTd /W2 /c)

    # Set the DLLEXPORT variable to export symbols
    target_compile_definitions(${PROJECT_NAME} PRIVATE WIN_EXPORT)
endif()


if(WIN32)
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")

    add_executable(${PROJECT_NAME}
            WIN32
            "src/main.cpp" ${SRC}
            ${APP_ICON_RESOURCE_WINDOWS}
        )
else()
    add_executable(${PROJECT_NAME} "src/main.cpp" ${SRC} )
endif()
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS /SUBSYSTEM:WINDOWS)    
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2main SDL2 )
else()
    target_link_libraries(${PROJECT_NAME} SDL2)
    message("unix ${SDL2_LIBRARIES}")
endif()
target_compile_definitions(${PROJECT_NAME} 
    PUBLIC
      $<$<CONFIG:Debug>:GAME_DEBUG>
      $<$<CONFIG:RelWithDebInfo>:GAME_DEBUG>
      $<$<CONFIG:Release>:GAME_RELEASE>
      $<$<CONFIG:MinSizeRel>:GAME_RELEASE>
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE sgl)
else()
    target_link_libraries(${PROJECT_NAME} sgl)
endif()
if(MINGW)
    IF(CMAKE_BUILD_TYPE MATCHES DEBUG) 
        message("debug mode")
        target_link_libraries(${PROJECT_NAME} SDL2)
    else()
        target_link_libraries(${PROJECT_NAME} mingw32 SDL2main SDL2 -mwindows)
    ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)
endif()
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_ttf)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_mixer)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} SDL2_ttf)
    target_link_libraries(${PROJECT_NAME} SDL2_image)
    target_link_libraries(${PROJECT_NAME} SDL2_mixer)
    target_link_libraries(${PROJECT_NAME} SDL2_gfx)
    target_link_libraries(${PROJECT_NAME} pthread)
endif()


add_custom_command(TARGET CapitalWorld PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/fonts/ $<TARGET_FILE_DIR:CapitalWorld>/fonts/)
add_custom_command(TARGET CapitalWorld PRE_BUILD
                  COMMAND ${CMAKE_COMMAND} -E copy_directory
                      ${CMAKE_SOURCE_DIR}/images/ $<TARGET_FILE_DIR:CapitalWorld>/images/)
add_custom_command(TARGET CapitalWorld PRE_BUILD
                  COMMAND ${CMAKE_COMMAND} -E copy_directory
                      ${CMAKE_SOURCE_DIR}/locale/ $<TARGET_FILE_DIR:CapitalWorld>/locale/)
add_custom_command(TARGET CapitalWorld PRE_BUILD
                      COMMAND ${CMAKE_COMMAND} -E copy_directory
                          ${CMAKE_SOURCE_DIR}/data/ $<TARGET_FILE_DIR:CapitalWorld>/data/)
add_custom_command(TARGET CapitalWorld PRE_BUILD
                          COMMAND ${CMAKE_COMMAND} -E copy_directory
                              ${CMAKE_SOURCE_DIR}/music/ $<TARGET_FILE_DIR:CapitalWorld>/music/)
if(WIN32)
    if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
        add_custom_command(TARGET CapitalWorld PRE_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_directory
                        ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2/lib/x64/ $<TARGET_FILE_DIR:CapitalWorld>/libs)    
    else()
        add_custom_command(TARGET CapitalWorld PRE_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_directory
                        ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2/lib/x86/ $<TARGET_FILE_DIR:CapitalWorld>/libs)
    endif()
endif()
INSTALL(TARGETS CapitalWorld DESTINATION capitalworld)
INSTALL(TARGETS sgl DESTINATION capitalworld)
INSTALL(DIRECTORY fonts DESTINATION capitalworld )
INSTALL(DIRECTORY images DESTINATION capitalworld )
INSTALL(DIRECTORY locale DESTINATION capitalworld )
INSTALL(DIRECTORY music DESTINATION capitalworld )
INSTALL(DIRECTORY data DESTINATION capitalworld )
if(WIN32)
    if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
        install(
            DIRECTORY
            ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2/lib/x64/
            DESTINATION
            capitalworld
        )
    else()
    install(
        DIRECTORY
        ${PROJECT_SOURCE_DIR}/deps/vcpp/SDL2/lib/x86/
        DESTINATION
        capitalworld
    )
    endif()
endif()
set(CPACK_COMPONENTS_ALL "CapitalWorld")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsdl2-2.0-0 (>= 2.0), libsdl2-ttf-2.0-0 (>= 2.0), libsdl2-image-2.0-0 (>= 2.0), libsdl2-gfx-1.0-0 (>= 1.0), libsdl2-mixer-2.0-0 (>= 2.0)")
set(CPACK_PACKAGE_VERSION "${GAME_VERSION_MAJOR}.${GAME_VERSION_MINOR}")
if(WIN32)
    SET(CPACK_GENERATOR "ZIP")
else()
    SET(CPACK_GENERATOR "DEB;TGZ")
    SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Stefan Lüdtke") #required
    SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/hisoka999/CapitalWorld")
    SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Capital World is a 2d isometric economy simulation game.")
    set(CPACK_DEBIAN_PACKAGE_SECTION "games")
endif()
INCLUDE(CPack)
