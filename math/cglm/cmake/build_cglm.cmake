include(CheckRetcode)

cmake_minimum_required(VERSION 3.10)

function(prepare)
    execute_process(COMMAND git clone https://github.com/recp/cglm.git ${ARGV0}
                    RESULT_VARIABLE retcode)
    check_retcode()

    execute_process(COMMAND mkdir build
                    RESULT_VARIABLE retcode
                    WORKING_DIRECTORY ${ARGV0})
    check_retcode()
endfunction()

function(build)
    execute_process(COMMAND git fetch
                    RESULT_VARIABLE retcode
                    WORKING_DIRECTORY ${ARGV0})
    check_retcode()

    execute_process(COMMAND git reset --hard e5d88f22a074fa3640c6e07c7a67aa0d8c7c9997
                    RESULT_VARIABLE retcode
                    WORKING_DIRECTORY ${ARGV0})
    check_retcode()

    execute_process(COMMAND cmake .. -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
                    RESULT_VARIABLE retcode
                    WORKING_DIRECTORY ${ARGV0}/build)
    check_retcode()

    execute_process(COMMAND cmake --build . --parallel
                    RESULT_VARIABLE retcode
                    WORKING_DIRECTORY ${ARGV0}/build)
    check_retcode()
endfunction()

function(set_paths)
    target_include_directories(${ARGV1} PUBLIC ${ARGV0}/include)
    target_link_directories(${ARGV1} PUBLIC ${ARGV0}/build)
    if (CMAKE_SYSTEM_NAME STREQUAL Windows)
        target_link_libraries(${ARGV1} cglm.dll)
    else()
        target_link_libraries(${ARGV1} cglm)
    endif()
endfunction()
