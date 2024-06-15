add_executable(diffuse diffuse.c ${ADDITIONAL_SOURCE_FILES} vs_diffuse.sc fs_diffuse.sc vs_diffuse_light.sc fs_diffuse_light.sc)

target_link_libraries(diffuse bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(diffuse PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(diffuse copy_resources)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_diffuse.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.diffuse.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE VERTEX
  SHADERS vs_diffuse.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.diffuse.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE VERTEX
  SHADERS vs_diffuse_light.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_diffuse_light.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)
