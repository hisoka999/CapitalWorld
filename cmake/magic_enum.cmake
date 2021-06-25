
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

#project(engine NONE)



  macro(fetch_magic_enum _download_module_path _download_root)
    set(MAGIC_ENUM_DOWNLOAD_ROOT ${_download_root})
    configure_file(
        ${_download_module_path}/magic_enum-download.cmake
        ${_download_root}/CMakeLists.txt
        @ONLY
        )
    unset(MAGIC_ENUM_DOWNLOAD_ROOT)

    execute_process(
        COMMAND
            "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY
            ${_download_root}
        )
    execute_process(
        COMMAND
            "${CMAKE_COMMAND}" --build .
        WORKING_DIRECTORY
            ${_download_root}
        )

    # # adds the targers: gtest, gtest_main, gmock, gmock_main
    add_subdirectory(
        ${_download_root}/magic_enum-src
        ${_download_root}/magic_enum-build
        )
endmacro()