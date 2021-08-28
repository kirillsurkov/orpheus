include(CheckRetcode)

cmake_minimum_required(VERSION 3.10)

function(prepare)
    execute_process(COMMAND git clone https://github.com/glennrp/libpng ${ARGV0}
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

    execute_process(COMMAND git reset --hard a37d4836519517bdce6cb9d956092321eca3e73b
                    RESULT_VARIABLE retcode
                    WORKING_DIRECTORY ${ARGV0})
    check_retcode()

    if (CMAKE_SYSTEM_NAME STREQUAL Windows)
        execute_process(COMMAND cmake .. -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} -DZLIB_INCLUDE_DIR=${ARGV0}/../zlib -DZLIB_LIBRARY=${ARGV0}/../zlib/build/libzlib.dll.a
                        RESULT_VARIABLE retcode
                        WORKING_DIRECTORY ${ARGV0}/build)
        check_retcode()
    else()
        execute_process(COMMAND cmake .. -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} -DZLIB_INCLUDE_DIR=${ARGV0}/../zlib -DZLIB_LIBRARY=${ARGV0}/../zlib/build/libz.a
                        RESULT_VARIABLE retcode
                        WORKING_DIRECTORY ${ARGV0}/build)
        check_retcode()
    endif()

    execute_process(COMMAND cmake --build . --parallel
                    RESULT_VARIABLE retcode
                    WORKING_DIRECTORY ${ARGV0}/build)
    check_retcode()
endfunction()

function(set_paths)
    target_include_directories(${ARGV1} PUBLIC ${ARGV0} ${ARGV0}/build)
    target_link_directories(${ARGV1} PUBLIC ${ARGV0}/build)
    if (CMAKE_SYSTEM_NAME STREQUAL Windows)
        target_link_libraries(${ARGV1} libpng.dll.a)
    else()
        target_link_libraries(${ARGV1} png)
    endif()
endfunction()
