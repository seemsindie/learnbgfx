add_executable(transformations transformations.c ${ADDITIONAL_SOURCE_FILES} vs_transformations.sc fs_transformations.sc)
add_executable(transformations_exercise_1 transformations_exercise_1.c ${ADDITIONAL_SOURCE_FILES} vs_transformations.sc fs_transformations.sc)
add_executable(transformations_exercise_2 transformations_exercise_2.c ${ADDITIONAL_SOURCE_FILES} vs_transformations.sc fs_transformations.sc)

target_link_libraries(transformations bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(transformations PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)

target_link_libraries(transformations_exercise_1 bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(transformations_exercise_1 PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)

target_link_libraries(transformations_exercise_2 bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(transformations_exercise_2 PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)

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


# Copy additional files for transformations
add_custom_command(
  TARGET transformations POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_CURRENT_SOURCE_DIR}/awesomeface.png
  $<TARGET_FILE_DIR:transformations>
  COMMENT "Copying awesomeface.png to build directory for transformations"
)

# Ensure container.jpg is copied only once
add_custom_command(
  TARGET transformations POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_CURRENT_SOURCE_DIR}/container.jpg
  $<TARGET_FILE_DIR:transformations>
  COMMENT "Copying container.jpg to build directory for transformations"
  DEPENDS textures
)

add_custom_command(
  TARGET transformations_exercise_1 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_CURRENT_SOURCE_DIR}/awesomeface.png
  $<TARGET_FILE_DIR:transformations_exercise_1>
  COMMENT "Copying awesomeface.png to build directory for transformations_exercise_1"
)

# Ensure container.jpg is copied only once
add_custom_command(
  TARGET transformations_exercise_1 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_CURRENT_SOURCE_DIR}/container.jpg
  $<TARGET_FILE_DIR:transformations_exercise_1>
  COMMENT "Copying container.jpg to build directory for transformations_exercise_1"
  DEPENDS textures
)

add_custom_command(
  TARGET transformations_exercise_2 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_CURRENT_SOURCE_DIR}/awesomeface.png
  $<TARGET_FILE_DIR:transformations_exercise_2>
  COMMENT "Copying awesomeface.png to build directory for transformations_exercise_2"
)

# Ensure container.jpg is copied only once
add_custom_command(
  TARGET transformations_exercise_2 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_CURRENT_SOURCE_DIR}/container.jpg
  $<TARGET_FILE_DIR:transformations_exercise_2>
  COMMENT "Copying container.jpg to build directory for transformations_exercise_2"
  DEPENDS textures
)