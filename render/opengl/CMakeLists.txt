include(ThirdParties)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(RenderOpenGL VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(${PROJECT_SOURCE_DIR}/include/render/opengl/Version.hpp.in ${PROJECT_SOURCE_DIR}/include/render/opengl/Version.hpp)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE INCLUDES "${PROJECT_SOURCE_DIR}/include/render/opengl/*.hpp")

add_library(RenderOpenGL ${SOURCES} ${INCLUDES})

target_include_directories(RenderOpenGL PUBLIC "${PROJECT_BINARY_DIR}" "./include" "../../engine/include")

set(OpenGL_GL_PREFERENCE "LEGACY")

find_package(OpenGL REQUIRED)
include_directories(RenderOpenGL ${OPENGL_INCLUDE_DIRS})
target_link_libraries(RenderOpenGL ${OPENGL_LIBRARIES})

third_parties(RenderOpenGL glew assimp zlib png)
