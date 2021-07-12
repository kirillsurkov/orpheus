cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(Orpheus VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(${PROJECT_SOURCE_DIR}/include/orpheus/Version.hpp.in ${PROJECT_SOURCE_DIR}/include/orpheus/Version.hpp)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE INCLUDES "${PROJECT_SOURCE_DIR}/include/orpheus/*.hpp")

add_library(Orpheus ${SOURCES} ${INCLUDES})

target_include_directories(Orpheus PUBLIC "${PROJECT_BINARY_DIR}" "./include" "../render/opengl/include" "../render/vulkan/include")

target_link_libraries(Orpheus MathCGLM)

target_link_libraries(Orpheus RenderOpenGL)
target_link_libraries(Orpheus RenderVulkan)

target_link_libraries(Orpheus PhysicsPhysX)
target_link_libraries(Orpheus PhysicsBullet)

find_package(Threads REQUIRED)
target_link_libraries(Orpheus Threads::Threads)
