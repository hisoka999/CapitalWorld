
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

#project(engine NONE)



  macro(fetch_engine _download_module_path _download_root)
    set(ENGINE_DOWNLOAD_ROOT ${_download_root})
    configure_file(
        ${_download_module_path}/engine-download.cmake
        ${_download_root}/CMakeLists.txt
        @ONLY
        )
    unset(ENGINE_DOWNLOAD_ROOT)

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
        ${_download_root}/engine-src
        ${_download_root}/engine-build
        )
endmacro()