# Conditional check for macOS
if(APPLE)
  set(ADDITIONAL_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/common/setup_metal_layer.mm ${CMAKE_CURRENT_SOURCE_DIR}/common/utils.c ${CMAKE_CURRENT_SOURCE_DIR}/common/camera.c ${CMAKE_CURRENT_SOURCE_DIR}/common/imgui/imgui_impl.c ${CMAKE_CURRENT_SOURCE_DIR}/common/macos_scale.m)
else()
  set(ADDITIONAL_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/common/utils.c ${CMAKE_CURRENT_SOURCE_DIR}/common/camera.c ${CMAKE_CURRENT_SOURCE_DIR}/common/imgui/imgui_impl.c)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(1.getting_started)
add_subdirectory(2.lighting)
add_subdirectory(bonus/1.imgui)