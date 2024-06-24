add_executable(shaders_uniform shaders_uniform.c ${ADDITIONAL_SOURCE_FILES} vs_uniform.sc fs_uniform.sc)

target_link_libraries(shaders_uniform bgfx bx bimg SDL2::SDL2-static SDL2_image cglm cimgui)
target_include_directories(shaders_uniform PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)

# Shader compilation (to header)
bgfx_compile_shader_to_header(
  TYPE VERTEX
  SHADERS vs_uniform.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_uniform.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)
