add_executable(imgui_example imgui_example.c ${ADDITIONAL_SOURCE_FILES} fs_imgui_specular.sc vs_imgui_specular.sc fs_imgui_specular_light.sc vs_imgui_specular_light.sc)

target_link_libraries(imgui_example bgfx bx bimg SDL2::SDL2-static SDL2_image cglm cimgui)
target_include_directories(imgui_example PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(imgui_example copy_resources)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_imgui_specular.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.imgui.specular.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE VERTEX
  SHADERS vs_imgui_specular.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.imgui.specular.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE VERTEX
  SHADERS vs_imgui_specular_light.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_imgui_specular_light.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)
