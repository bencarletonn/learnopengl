add_library(shaders)

target_sources(shaders PRIVATE
  shaders.cpp
)

target_include_directories(shaders PUBLIC
  "${CMAKE_SOURCE_DIR}/include"
)

target_link_libraries(shaders PUBLIC
  glad
)
