cmake_minimum_required(VERSION 3.10)

project(Orpheus VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(${PROJECT_SOURCE_DIR}/include/Version.hpp.in ${PROJECT_SOURCE_DIR}/include/Version.hpp)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE INCLUDES "${PROJECT_SOURCE_DIR}/include/*.hpp")

add_executable(Orpheus ${SOURCES} ${INCLUDES})

target_include_directories(Orpheus PUBLIC "${PROJECT_BINARY_DIR}" "./include")

find_package(SDL2 REQUIRED)
include_directories(Orpheus ${SDL2_INCLUDE_DIRS})
target_link_libraries(Orpheus ${SDL2_LIBRARIES})

find_package(OpenGL REQUIRED)
include_directories(Orpheus ${OPENGL_INCLUDE_DIRS})
target_link_libraries(Orpheus ${OPENGL_LIBRARIES})
