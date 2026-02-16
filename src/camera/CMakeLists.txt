add_library(camera)

target_sources(camera PRIVATE
  camera.cpp
)

target_include_directories(camera PUBLIC
  "${CMAKE_SOURCE_DIR}/include"
)

target_link_libraries(camera PUBLIC
  glad
  glm
)
