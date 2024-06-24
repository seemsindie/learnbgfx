add_executable(ambient ambient.c ${ADDITIONAL_SOURCE_FILES} vs_light_cube.sc fs_ambient.sc vs_ambient_light.sc fs_ambient_light.sc)

target_link_libraries(ambient bgfx bx bimg SDL2::SDL2-static SDL2_image cglm cimgui)
target_include_directories(ambient PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(ambient copy_resources)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_light_cube.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_ambient.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE VERTEX
  SHADERS vs_ambient_light.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_ambient_light.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)
