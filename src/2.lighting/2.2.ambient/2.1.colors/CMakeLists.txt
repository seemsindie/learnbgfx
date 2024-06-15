add_executable(colors colors.c ${ADDITIONAL_SOURCE_FILES} vs_light_cube.sc fs_light_cube.sc vs_light.sc fs_light.sc)
add_executable(ambient ambient.c ${ADDITIONAL_SOURCE_FILES} vs_light_cube.sc fs_ambient.sc vs_light.sc fs_light.sc)
add_executable(diffuse diffuse.c ${ADDITIONAL_SOURCE_FILES} vs_diffuse.sc fs_diffuse.sc vs_light.sc fs_light.sc)
add_executable(specular specular.c ${ADDITIONAL_SOURCE_FILES} vs_specular.sc fs_specular.sc vs_light.sc fs_light.sc)

target_link_libraries(colors bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(colors PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(colors copy_resources)

target_link_libraries(ambient bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(ambient PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(ambient copy_resources)

target_link_libraries(diffuse bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(diffuse PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(diffuse copy_resources)

target_link_libraries(specular bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(specular PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(specular copy_resources)

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

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_ambient.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

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
  SHADERS vs_light.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_light.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)
