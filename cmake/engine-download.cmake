cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(engine-download NONE)


include(ExternalProject)


ExternalProject_Add(
  engine
  SOURCE_DIR "@ENGINE_DOWNLOAD_ROOT@/engine-src"
  BINARY_DIR "@ENGINE_DOWNLOAD_ROOT@/engine-build"
  GIT_REPOSITORY
    https://github.com/hisoka999/libsgl.git
  GIT_TAG
    origin/master
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  TEST_COMMAND ""
  )
