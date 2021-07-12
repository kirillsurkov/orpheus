include(ThirdParties)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(PhysicsPhysX VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(${PROJECT_SOURCE_DIR}/include/physics/physx/Version.hpp.in ${PROJECT_SOURCE_DIR}/include/physics/physx/Version.hpp)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE INCLUDES "${PROJECT_SOURCE_DIR}/include/physics/physx/*.hpp")

add_library(PhysicsPhysX ${SOURCES} ${INCLUDES})

target_include_directories(PhysicsPhysX PUBLIC "${PROJECT_BINARY_DIR}" "./include" "../../engine/include")

third_parties(PhysicsPhysX physx)
