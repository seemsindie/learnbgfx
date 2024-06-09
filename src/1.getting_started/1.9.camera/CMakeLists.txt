add_executable(camera_lookat camera_lookat.c ${ADDITIONAL_SOURCE_FILES} vs_camera_lookat.sc fs_camera_lookat.sc)
add_executable(camera_keyboard_move camera_keyboard_move.c ${ADDITIONAL_SOURCE_FILES} vs_camera_lookat.sc fs_camera_lookat.sc)
add_executable(camera_keyboard_mouse_move camera_keyboard_mouse_move.c ${ADDITIONAL_SOURCE_FILES} vs_camera_lookat.sc fs_camera_lookat.sc)
add_executable(camera_struct camera_struct.c ${ADDITIONAL_SOURCE_FILES} vs_camera_lookat.sc fs_camera_lookat.sc)

target_link_libraries(camera_lookat bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(camera_lookat PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(camera_lookat copy_resources)

target_link_libraries(camera_keyboard_move bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(camera_keyboard_move PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(camera_keyboard_move copy_resources)

target_link_libraries(camera_keyboard_mouse_move bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(camera_keyboard_mouse_move PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(camera_keyboard_mouse_move copy_resources)

target_link_libraries(camera_struct bgfx bx bimg SDL2::SDL2-static SDL2_image cglm)
target_include_directories(camera_struct PRIVATE ${CMAKE_BINARY_DIR}/include/generated/shaders)
add_dependencies(camera_struct copy_resources)

bgfx_compile_shader_to_header(
  TYPE VERTEX
  SHADERS vs_camera_lookat.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)

bgfx_compile_shader_to_header(
  TYPE FRAGMENT
  SHADERS fs_camera_lookat.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/include/generated/shaders
)
