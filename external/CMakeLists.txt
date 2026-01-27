add_library(glad STATIC)

target_sources(glad PRIVATE
  src/glad.c
)

target_include_directories(glad
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(stb STATIC)

target_sources(stb PRIVATE
  src/stb.cpp
)

target_include_directories(stb
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
