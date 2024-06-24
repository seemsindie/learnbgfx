add_executable(coordinate_system_plane coordinate_system_plane.c ${ADDITIONAL_SOURCE_FILES} vs_coordinate_system.sc fs_coordinate_system.sc)
add_executable(coordinate_system_cube coordinate_system_cube.c ${ADDITIONAL_SOURCE_FILES} vs_coordinate_system.sc fs_coordinate_system.sc)
add_executable(coordinate_system_even_more_cubes coordinate_system_even_more_cubes.c ${ADDITIONAL_SOURCE_FILES} vs_coordinate_system.sc fs_coordinate_system.sc)

target_link_libraries(coordinate_system_plane bgfx bx bimg SDL2::SDL2-static SDL2_image cglm cimgui)
target_include_directories(coordinate_system_plane PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(coordinate_system_plane copy_resources)

target_link_libraries(coordinate_system_cube bgfx bx bimg SDL2::SDL2-static SDL2_image cglm cimgui)
target_include_directories(coordinate_system_cube PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(coordinate_system_cube copy_resources)

target_link_libraries(coordinate_system_even_more_cubes bgfx bx bimg SDL2::SDL2-static SDL2_image cglm cimgui)
target_include_directories(coordinate_system_even_more_cubes PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(coordinate_system_even_more_cubes copy_resources)

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
