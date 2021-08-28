include(CheckRetcode)

cmake_minimum_required(VERSION 3.10)

function(prepare)
    execute_process(COMMAND git clone https://github.com/bulletphysics/bullet3 ${ARGV0}
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

    execute_process(COMMAND git reset --hard 41158a4be8f95637ec60574403b3b39be53d99c7
                    RESULT_VARIABLE retcode
                    WORKING_DIRECTORY ${ARGV0})
    check_retcode()

    execute_process(COMMAND cmake .. -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} -DBUILD_BULLET2_DEMOS=OFF -DBUILD_UNIT_TESTS=OFF
                    RESULT_VARIABLE retcode
                    WORKING_DIRECTORY ${ARGV0}/build)
    check_retcode()

    execute_process(COMMAND cmake --build . --parallel
                    RESULT_VARIABLE retcode
                    WORKING_DIRECTORY ${ARGV0}/build)
    check_retcode()
endfunction()

function(set_paths)
    target_include_directories(${ARGV1} PUBLIC ${ARGV0}/src)
    target_link_directories(${ARGV1} PUBLIC ${ARGV0}/build/lib)
    if (CMAKE_SYSTEM_NAME STREQUAL Windows)
#        target_link_libraries(${ARGV1} libBullet3Common.a libBullet3Dynamics.a libBullet3Geometry.a)
    else()
#        target_link_libraries(${ARGV1} Bullet3Common Bullet3Dynamics Bullet3Geometry)
    endif()
endfunction()
