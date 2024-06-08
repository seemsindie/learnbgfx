cmake_minimum_required(VERSION 3.10)
project(learnbgfx VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)

set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/3rdparty)
set(BUILD_SHARED_LIBS OFF)
set(SDL2IMAGE_BUILD_SHARED_LIBS OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

# option(CGLM_SHARED "Shared build" OFF)
# option(CGLM_STATIC "Static build" ON)
set(CGLM_SHARED OFF)
set(CGLM_STATIC ON)

# Fetch bgfx
FetchContent_Declare(
  bgfx
  GIT_REPOSITORY https://github.com/bkaradzic/bgfx.cmake
  GIT_TAG        origin/master
  SOURCE_DIR     "${FETCHCONTENT_BASE_DIR}/bgfx"
)
FetchContent_MakeAvailable(bgfx)

# Fetch SDL2
FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL
  GIT_TAG        origin/SDL2
  SOURCE_DIR     "${FETCHCONTENT_BASE_DIR}/SDL2"
)
FetchContent_MakeAvailable(SDL2)

# Fetch SDL2_image
FetchContent_Declare(
  SDL2_image
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
  GIT_TAG        origin/SDL2
  SOURCE_DIR     "${FETCHCONTENT_BASE_DIR}/SDL2_image"
)
FetchContent_MakeAvailable(SDL2_image)

# Fetch cglm
FetchContent_Declare(
  cglm
  GIT_REPOSITORY https://github.com/recp/cglm.git
  GIT_TAG        origin/master
  SOURCE_DIR     "${FETCHCONTENT_BASE_DIR}/cglm"
)
FetchContent_MakeAvailable(cglm)

include_directories(${CMAKE_SOURCE_DIR}/include/common)

add_subdirectory(src)
