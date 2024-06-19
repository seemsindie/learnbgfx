add_executable(materials materials.c ${ADDITIONAL_SOURCE_FILES} vs_materials.sc fs_materials.sc vs_materials_light.sc fs_materials_light.sc)

target_link_libraries(materials bgfx bx bimg SDL2::SDL2-static SDL2_image cglm cimgui)
target_include_directories(materials PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(materials copy_resources)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_materials.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.materials.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE VERTEX
  SHADERS vs_materials.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.materials.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE VERTEX
  SHADERS vs_materials_light.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_materials_light.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)
