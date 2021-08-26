include(ThirdParties)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(RenderVulkan VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(${PROJECT_SOURCE_DIR}/include/render/vulkan/Version.hpp.in ${PROJECT_SOURCE_DIR}/include/render/vulkan/Version.hpp)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE INCLUDES "${PROJECT_SOURCE_DIR}/include/render/vulkan/*.hpp")

add_library(RenderVulkan ${SOURCES} ${INCLUDES})

target_include_directories(RenderVulkan PUBLIC "${PROJECT_BINARY_DIR}" "./include" "../../engine/include")

find_package(Vulkan REQUIRED)
include_directories(RenderVulkan ${Vulkan_INCLUDE_DIRS})
target_link_libraries(RenderVulkan ${Vulkan_LIBRARIES})
