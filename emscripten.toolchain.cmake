# emscripten.toolchain.cmake

set(CMAKE_SYSTEM_NAME Emscripten)
set(CMAKE_SYSTEM_VERSION 1)

# Specify the compiler to use
set(CMAKE_C_COMPILER "emcc")
set(CMAKE_CXX_COMPILER "em++")
set(CMAKE_AR "emar")
set(CMAKE_RANLIB "emranlib")

# Specify the emscripten root directory
# set(EMSCRIPTEN_ROOT "/path/to/emsdk/upstream/emscripten")

# Setup linker flags for Emscripten
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/data@/data")

# Add Emscripten libraries
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s FULL_ES2=1 -s FULL_ES3=1 -s ASYNCIFY=1")

# Enable optimization
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
