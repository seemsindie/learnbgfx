add_executable(coordinate_system_plane coordinate_system_plane.c ${ADDITIONAL_SOURCE_FILES} vs_coordinate_system.sc fs_coordinate_system.sc)
add_executable(coordinate_system_cube coordinate_system_cube.c ${ADDITIONAL_SOURCE_FILES} vs_coordinate_system.sc fs_coordinate_system.sc)
add_executable(coordinate_system_even_more_cubes coordinate_system_even_more_cubes.c ${ADDITIONAL_SOURCE_FILES} vs_coordinate_system.sc fs_coordinate_system.sc)

target_link_libraries(coordinate_system_plane bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(coordinate_system_plane PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)

target_link_libraries(coordinate_system_cube bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(coordinate_system_cube PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)

target_link_libraries(coordinate_system_even_more_cubes bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(coordinate_system_even_more_cubes PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)

bgfx_compile_shader_to_header(
  TYPE VERTEX
  SHADERS vs_coordinate_system.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_coordinate_system.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

# Copy additional files for coordinate_system
add_custom_command(
  TARGET coordinate_system_plane POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_CURRENT_SOURCE_DIR}/awesomeface.png
  $<TARGET_FILE_DIR:coordinate_system_plane>
  COMMENT "Copying awesomeface.png to build directory for coordinate_system_plane"
)

# Ensure container.jpg is copied only once
add_custom_command(
  TARGET coordinate_system_plane POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_CURRENT_SOURCE_DIR}/container.jpg
  $<TARGET_FILE_DIR:coordinate_system_plane>
  COMMENT "Copying container.jpg to build directory for coordinate_system_plane"
  DEPENDS textures
)