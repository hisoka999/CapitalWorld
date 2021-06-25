cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(magic_enum-download NONE)


include(ExternalProject)


ExternalProject_Add(
    magic_enum
  SOURCE_DIR "@MAGIC_ENUM_DOWNLOAD_ROOT@/magic_enum-src"
  BINARY_DIR "@MAGIC_ENUM_DOWNLOAD_ROOT@/magic_enum-build"
  GIT_REPOSITORY
    https://github.com/neargye/magic_enum.git
  GIT_TAG
    v0.7.0
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  TEST_COMMAND ""
  )
