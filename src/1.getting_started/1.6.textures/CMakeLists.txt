add_executable(textures textures.c ${ADDITIONAL_SOURCE_FILES} vs_textures.sc fs_textures.sc)
add_executable(textures_combined textures_combined.c ${ADDITIONAL_SOURCE_FILES} vs_textures_combined.sc fs_textures_combined.sc)
add_executable(textures_exercise_1 textures_exercise_1.c ${ADDITIONAL_SOURCE_FILES} vs_textures_combined.sc fs_textures_exercise_1.sc)
add_executable(textures_exercise_4 textures_exercise_4.c ${ADDITIONAL_SOURCE_FILES} vs_textures_combined.sc fs_textures_exercise_4.sc)

target_link_libraries(textures bgfx bx bimg SDL2::SDL2-static SDL2_image cglm cimgui)
target_include_directories(textures PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(textures copy_resources)

target_link_libraries(textures_combined bgfx bx bimg SDL2::SDL2-static SDL2_image cglm cimgui)
target_include_directories(textures_combined PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(textures_combined copy_resources)

target_link_libraries(textures_exercise_1 bgfx bx bimg SDL2::SDL2-static SDL2_image cglm cimgui)
target_include_directories(textures_exercise_1 PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(textures_exercise_1 copy_resources)

target_link_libraries(textures_exercise_4 bgfx bx bimg SDL2::SDL2-static SDL2_image cglm cimgui)
target_include_directories(textures_exercise_4 PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(textures_exercise_4 copy_resources)

# Shader compilation (to header)
bgfx_compile_shader_to_header(
  TYPE VERTEX
  SHADERS vs_textures.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_textures.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE VERTEX
  SHADERS vs_textures_combined.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_textures_combined.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_textures_exercise_1.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_textures_exercise_4.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)
