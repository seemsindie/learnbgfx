if (WIN32)
    add_executable(hello_sdl_window WIN32 hello_sdl_window.c ${ADDITIONAL_SOURCE_FILES})
    set_target_properties(hello_sdl_window PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
else()
    add_executable(hello_sdl_window hello_sdl_window.c ${ADDITIONAL_SOURCE_FILES})
endif()

target_link_libraries(hello_sdl_window bgfx bx bimg SDL2::SDL2-static SDL2_image cglm gdi32)