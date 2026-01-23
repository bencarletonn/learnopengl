add_subdirectory(shaders)

add_executable(scene)

target_sources(scene PRIVATE
  main.cpp
)

target_include_directories(scene PRIVATE 
  "${CMAKE_SOURCE_DIR}/include"
  "${LOCAL_PREFIX}/include"
)

target_link_directories(scene PRIVATE
  "${LOCAL_PREFIX}/lib"
)

target_link_libraries(scene PRIVATE
  shaders
  glad
  glfw3
  "-framework Cocoa"
  "-framework IOKit"
  "-framework CoreFoundation"
  "-framework CoreVideo"
  "-framework QuartzCore"
  "-framework OpenGL"
)
