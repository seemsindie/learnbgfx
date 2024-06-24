add_executable(transformations transformations.c ${ADDITIONAL_SOURCE_FILES} vs_transformations.sc fs_transformations.sc)
add_executable(transformations_exercise_1 transformations_exercise_1.c ${ADDITIONAL_SOURCE_FILES} vs_transformations.sc fs_transformations.sc)
add_executable(transformations_exercise_2 transformations_exercise_2.c ${ADDITIONAL_SOURCE_FILES} vs_transformations.sc fs_transformations.sc)

target_link_libraries(transformations bgfx bx bimg SDL2::SDL2-static SDL2_image cglm cimgui)
target_include_directories(transformations PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(transformations copy_resources)

target_link_libraries(transformations_exercise_1 bgfx bx bimg SDL2::SDL2-static SDL2_image cglm cimgui)
target_include_directories(transformations_exercise_1 PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(transformations_exercise_1 copy_resources)

target_link_libraries(transformations_exercise_2 bgfx bx bimg SDL2::SDL2-static SDL2_image cglm cimgui)
target_include_directories(transformations_exercise_2 PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(transformations_exercise_2 copy_resources)

# Shader compilation (to header)
bgfx_compile_shader_to_header(
  TYPE VERTEX
  SHADERS vs_transformations.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_transformations.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)
