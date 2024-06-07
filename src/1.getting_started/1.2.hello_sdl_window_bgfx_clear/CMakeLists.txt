
add_executable(hello_sdl_window_bgfx_clear hello_sdl_window_bgfx_clear.c ${ADDITIONAL_SOURCE_FILES})
target_link_libraries(hello_sdl_window_bgfx_clear bgfx bx bimg SDL2::SDL2-static SDL2_image)