add_executable(shaders_struct shaders_struct.c ${ADDITIONAL_SOURCE_FILES} vs_struct.sc fs_struct.sc)

target_link_libraries(shaders_struct bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(shaders_struct PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)

# Shader compilation (to header)
bgfx_compile_shader_to_header(
  TYPE VERTEX
  SHADERS vs_struct.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_struct.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)
