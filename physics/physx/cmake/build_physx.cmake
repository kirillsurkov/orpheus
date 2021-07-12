include(CheckRetcode)

cmake_minimum_required(VERSION 3.10)

function(prepare)
    execute_process(COMMAND git clone https://github.com/NVIDIAGameWorks/PhysX.git ${ARGV0}
                    RESULT_VARIABLE retcode)
    check_retcode()
endfunction()

function(build)
    execute_process(COMMAND git fetch
                    RESULT_VARIABLE retcode
                    WORKING_DIRECTORY ${ARGV0})
    check_retcode()

    execute_process(COMMAND git reset --hard c3d5537bdebd6f5cd82fcaf87474b838fe6fd5fa
                    RESULT_VARIABLE retcode
                    WORKING_DIRECTORY ${ARGV0})
    check_retcode()

    execute_process(COMMAND git apply ${PROJECT_SOURCE_DIR}/physx_patch.diff
                    RESULT_VARIABLE retcode
                    WORKING_DIRECTORY ${ARGV0}/physx)
    check_retcode()

    if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        execute_process(COMMAND bash generate_projects.sh windows
                        result_variable retcode
                        working_directory ${argv0}/physx)
        check_retcode()
    else()
        execute_process(COMMAND bash generate_projects.sh linux
                        result_variable retcode
                        working_directory ${argv0}/physx)
        check_retcode()
    endif()

    execute_process(COMMAND cmake --build . --parallel
                    RESULT_VARIABLE retcode
                    WORKING_DIRECTORY ${ARGV0}/physx/compiler/linux-release)
    check_retcode()
endfunction()

function(set_paths)
    target_include_directories(${ARGV1} PUBLIC ${ARGV0}/physx/include ${ARGV0}/pxshared/include)
    target_link_directories(${ARGV1} PUBLIC ${ARGV0}/physx/bin/linux.clang/release)
    target_link_libraries(${ARGV1} PhysXExtensions_static_64 PhysX_static_64 PhysXFoundation_static_64 PhysXCommon_static_64 PhysXCooking_static_64 ${CMAKE_DL_LIBS})
endfunction()
