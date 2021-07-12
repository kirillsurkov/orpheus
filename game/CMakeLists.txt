cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(Game VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(${PROJECT_SOURCE_DIR}/include/Version.hpp.in ${PROJECT_SOURCE_DIR}/include/Version.hpp)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE INCLUDES "${PROJECT_SOURCE_DIR}/include/*.hpp")

add_executable(Game ${SOURCES} ${INCLUDES})

target_include_directories(Game PUBLIC "${PROJECT_BINARY_DIR}" "./include" "../engine/include")

add_custom_target(CopyResources COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res)
add_dependencies(Game CopyResources)

target_link_libraries(Game Orpheus)

third_parties(Game sdl2)
