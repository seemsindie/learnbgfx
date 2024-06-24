add_executable(specular specular.c ${ADDITIONAL_SOURCE_FILES} vs_specular.sc fs_specular.sc vs_specular_light.sc fs_specular_light.sc)

target_link_libraries(specular bgfx bx bimg SDL2::SDL2-static SDL2_image cglm cimgui)
target_include_directories(specular PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(specular copy_resources)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_specular.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.specular.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE VERTEX
  SHADERS vs_specular.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.specular.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE VERTEX
  SHADERS vs_specular_light.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_specular_light.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)
