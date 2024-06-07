# Conditional check for macOS
if(APPLE)
  set(ADDITIONAL_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/common/setup_metal_layer.mm ${CMAKE_CURRENT_SOURCE_DIR}/common/utils.c ${CMAKE_CURRENT_SOURCE_DIR}/common/camera.c)
else()
  set(ADDITIONAL_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/common/utils.c ${CMAKE_CURRENT_SOURCE_DIR}/common/camera.c)
endif()

add_subdirectory(1.getting_started)