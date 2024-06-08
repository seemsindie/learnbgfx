add_executable(textures textures.c ${ADDITIONAL_SOURCE_FILES} vs_textures.sc fs_textures.sc)
add_executable(textures_combined textures_combined.c ${ADDITIONAL_SOURCE_FILES} vs_textures_combined.sc fs_textures_combined.sc)
add_executable(textures_exercise_1 textures_exercise_1.c ${ADDITIONAL_SOURCE_FILES} vs_textures_combined.sc fs_textures_exercise_1.sc)
add_executable(textures_exercise_4 textures_exercise_4.c ${ADDITIONAL_SOURCE_FILES} vs_textures_combined.sc fs_textures_exercise_4.sc)

target_link_libraries(textures bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(textures PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)

target_link_libraries(textures_combined bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(textures_combined PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)

target_link_libraries(textures_exercise_1 bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(textures_exercise_1 PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)

target_link_libraries(textures_exercise_4 bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(textures_exercise_4 PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)

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

# Copy the container.jpg to the build directory
add_custom_command(
  TARGET textures POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_CURRENT_SOURCE_DIR}/container.jpg
  $<TARGET_FILE_DIR:textures>
)

# Copy additional files for textures_combined
add_custom_command(
  TARGET textures_combined POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_CURRENT_SOURCE_DIR}/awesomeface.png
  $<TARGET_FILE_DIR:textures_combined>
  COMMENT "Copying awesomeface.png to build directory for textures_combined"
)

# Ensure container.jpg is copied only once
add_custom_command(
  TARGET textures_combined POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_CURRENT_SOURCE_DIR}/container.jpg
  $<TARGET_FILE_DIR:textures_combined>
  COMMENT "Copying container.jpg to build directory for textures_combined"
  DEPENDS textures
)