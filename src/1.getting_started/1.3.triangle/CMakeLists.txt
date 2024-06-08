add_executable(triangle triangle.c ${ADDITIONAL_SOURCE_FILES} vs_triangle.sc fs_triangle.sc)
add_executable(triangle_indexed triangle_indexed.c ${ADDITIONAL_SOURCE_FILES} vs_triangle.sc fs_triangle.sc)
add_executable(quad quad.c ${ADDITIONAL_SOURCE_FILES} vs_triangle.sc fs_triangle.sc)
add_executable(quad_proper quad_proper.c ${ADDITIONAL_SOURCE_FILES} vs_triangle.sc fs_triangle.sc)

target_link_libraries(triangle bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(triangle PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)

target_link_libraries(triangle_indexed bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(triangle_indexed PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)

target_link_libraries(quad bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(quad PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)

target_link_libraries(quad_proper bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(quad_proper PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)

# Shader compilation (to header)
bgfx_compile_shader_to_header(
  TYPE VERTEX
  SHADERS vs_triangle.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_triangle.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)
