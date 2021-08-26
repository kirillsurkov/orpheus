include(CheckRetcode)

cmake_minimum_required(VERSION 3.10)

function(prepare)
    execute_process(COMMAND git clone https://github.com/libsdl-org/SDL.git ${ARGV0}
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

    execute_process(COMMAND git reset --hard 4cd981609b50ed273d80c635c1ca4c1e5518fb21
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
    target_link_directories(${ARGV1} PUBLIC ${ARGV0}/build)
    if (CMAKE_SYSTEM_NAME STREQUAL Windows)
        target_link_libraries(${ARGV1} SDL2.dll SDL2main)
    else()
        target_link_libraries(${ARGV1} SDL2 SDL2main dl)
    endif()
endfunction()
