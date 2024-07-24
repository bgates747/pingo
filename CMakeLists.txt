cmake_minimum_required(VERSION 3.0)
project(pingo)

set(CMAKE_BUILD_TYPE Debug)

# Pingo rendering library
file(GLOB_RECURSE render_src render/*.h render/*.c)
file(GLOB_RECURSE math_src math/*.h math/*.c)
add_library(pingo SHARED ${render_src} ${math_src})
target_link_libraries(pingo m)

# Assets library 
file(GLOB_RECURSE assets_src assets/*.h assets/*.c)
add_library(assets SHARED ${assets_src})
target_link_libraries(assets pingo m)

# Examples sources
file(GLOB_RECURSE linux_window_src example/linux_window/*.h example/linux_window/*.c)
file(GLOB_RECURSE linux_lookaround_src example/linux_lookaround/*.h example/linux_lookaround/*.c)

include_directories(./)

if (UNIX)
  add_executable(linux_window ${linux_window_src})
  target_link_libraries(linux_window pingo assets X11)
  configure_file(${CMAKE_SOURCE_DIR}/assets/viking.rgba ${CMAKE_BINARY_DIR}/assets/viking.rgba COPYONLY)

  add_executable(linux_lookaround ${linux_lookaround_src})
  target_link_libraries(linux_lookaround pingo assets X11)
  configure_file(${CMAKE_SOURCE_DIR}/assets/viking.rgba ${CMAKE_BINARY_DIR}/assets/viking.rgba COPYONLY)
endif (UNIX)