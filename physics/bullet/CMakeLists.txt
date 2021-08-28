include(ThirdParties)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(PhysicsBullet VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(${PROJECT_SOURCE_DIR}/include/physics/bullet/Version.hpp.in ${PROJECT_SOURCE_DIR}/include/physics/bullet/Version.hpp)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE INCLUDES "${PROJECT_SOURCE_DIR}/include/physics/bullet/*.hpp")

add_library(PhysicsBullet ${SOURCES} ${INCLUDES})

target_include_directories(PhysicsBullet PUBLIC "${PROJECT_BINARY_DIR}" "./include" "../../engine/include")

third_parties(PhysicsBullet bullet)
