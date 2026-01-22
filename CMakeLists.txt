cmake_minimum_required(VERSION 3.23)
project(learnopengl LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LOCAL_PREFIX "$ENV{HOME}/.local")

add_subdirectory(external)
add_subdirectory(src)
