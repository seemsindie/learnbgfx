add_executable(imgui_example imgui_example.c ${ADDITIONAL_SOURCE_FILES})

target_link_libraries(imgui_example bgfx bx bimg SDL2::SDL2-static SDL2_image cglm cimgui "-framework Cocoa")
target_include_directories(imgui_example PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(imgui_example copy_resources)
