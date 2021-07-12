include(ThirdParties)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(MathCGLM VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(${PROJECT_SOURCE_DIR}/include/math/cglm/Version.hpp.in ${PROJECT_SOURCE_DIR}/include/math/cglm/Version.hpp)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE INCLUDES "${PROJECT_SOURCE_DIR}/include/math/cglm/*.hpp")

add_library(MathCGLM ${SOURCES} ${INCLUDES})

target_include_directories(MathCGLM PUBLIC "${PROJECT_BINARY_DIR}" "./include" "../../engine/include")

third_parties(MathCGLM cglm)
