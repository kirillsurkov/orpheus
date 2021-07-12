cmake_minimum_required(VERSION 3.10)

function(prepare)
    execute_process(COMMAND git clone https://github.com/recp/cglm.git ${ARGV0})
    execute_process(COMMAND mkdir build
                    WORKING_DIRECTORY ${ARGV0})
endfunction()

function(build)
    execute_process(COMMAND git fetch
                    WORKING_DIRECTORY ${ARGV0})

    execute_process(COMMAND git reset --hard e5d88f22a074fa3640c6e07c7a67aa0d8c7c9997
                    WORKING_DIRECTORY ${ARGV0})

    execute_process(COMMAND cmake ..
                    WORKING_DIRECTORY ${ARGV0}/build)

    execute_process(COMMAND make
                    WORKING_DIRECTORY ${ARGV0}/build)
endfunction()

function(set_paths)
    target_include_directories(${ARGV1} PUBLIC ${ARGV0}/include)
    target_link_directories(${ARGV1} PUBLIC ${ARGV0}/build)
    target_link_libraries(${ARGV1} cglm)
endfunction()
