add_executable(colors colors.c ${ADDITIONAL_SOURCE_FILES} vs_light_cube.sc fs_light_cube.sc)

target_link_libraries(colors bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(colors PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(colors copy_resources)

bgfx_compile_shader_to_header(
  TYPE VERTEX
  SHADERS vs_light_cube.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_light_cube.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

# bgfx_compile_shader_to_header(
#   TYPE VERTEX
#   SHADERS vs_light.sc
#   VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
#   OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
# )

# bgfx_compile_shader_to_header(
#   TYPE FRAGMENT
#   SHADERS fs_light.sc
#   VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
#   OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
# )
