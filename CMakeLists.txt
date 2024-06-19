cmake_minimum_required(VERSION 3.10)
project(learnbgfx VERSION 0.1.0 LANGUAGES C CXX)

# set(CMAKE_C_STANDARD 99)
# set(CMAKE_C_STANDARD_REQUIRED ON)
# set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/3rdparty)
set(BUILD_SHARED_LIBS OFF)
set(SDL2IMAGE_BUILD_SHARED_LIBS OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

# Resources
set(RESOURCES_SOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")
set(RESOURCES_DEST_DIR "${CMAKE_BINARY_DIR}/resources")


# Emscripten-specific settings
if(EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s FULL_ES2=1 -s FULL_ES3=1 -s ASYNCIFY=1")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s FULL_ES2=1 -s FULL_ES3=1 -s ASYNCIFY=1 --preload-file ${CMAKE_SOURCE_DIR}/data@/data")
endif()


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

#Fetch cimgui
FetchContent_Declare(
  cimgui
  GIT_REPOSITORY https://github.com/cimgui/cimgui.git
  GIT_TAG        origin/master
  SOURCE_DIR     "${FETCHCONTENT_BASE_DIR}/cimgui"
)
FetchContent_MakeAvailable(cimgui)

include_directories(${CMAKE_SOURCE_DIR}/include/common)

add_subdirectory(src)

# Copy resources
add_custom_target(copy_resources ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${RESOURCES_SOURCE_DIR} ${RESOURCES_DEST_DIR}
  DEPENDS ${RESOURCES_SOURCE_DIR}
  COMMENT "Copying resources to build directory"
)
