include(CheckRetcode)

cmake_minimum_required(VERSION 3.10)

function(prepare)
    execute_process(COMMAND git clone https://github.com/kirillsurkov/assimp.git ${ARGV0}
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

    execute_process(COMMAND git reset --hard 5ca2cbb7ae0bf43be94683d0f244b44e2a7d3a51
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
    target_include_directories(${ARGV1} PUBLIC ${ARGV0}/include ${ARGV0}/build/include)
    if (CMAKE_SYSTEM_NAME STREQUAL Windows)
        target_link_directories(${ARGV1} PUBLIC ${ARGV0}/build/lib)
        target_link_libraries(${ARGV1} assimp.dll)
    else()
        target_link_directories(${ARGV1} PUBLIC ${ARGV0}/build/bin)
        target_link_libraries(${ARGV1} assimp)
    endif()
endfunction()
