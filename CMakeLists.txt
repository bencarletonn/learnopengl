cmake_minimum_required(VERSION 3.23)
project(learnopengl LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LOCAL_PREFIX "$ENV{HOME}/.local")

add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC include)

add_executable(scene)

target_sources(scene PRIVATE
	src/main.cpp
)

target_include_directories(scene PRIVATE
	"${LOCAL_PREFIX}/include"
)

target_link_directories(scene PRIVATE
	"${LOCAL_PREFIX}/lib"
)

target_link_libraries(scene PRIVATE	
	glad
	glfw3
	"-framework OpenGL"
)
