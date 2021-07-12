cmake_minimum_required(VERSION 3.10)

function(prepare)
    execute_process(COMMAND git clone https://github.com/nigels-com/glew.git ${ARGV0})
endfunction()

function(build)
    execute_process(COMMAND git fetch
                    WORKING_DIRECTORY ${ARGV0})

    execute_process(COMMAND git reset --hard 9fb23c3e61cbd2d581e33ff7d8579b572b38ee26
                    WORKING_DIRECTORY ${ARGV0})

    set(ENV{PYTHON} python3)
    execute_process(COMMAND make
                    WORKING_DIRECTORY ${ARGV0}/auto)

    execute_process(COMMAND cmake ./cmake
                    WORKING_DIRECTORY ${ARGV0}/build)

    execute_process(COMMAND make
                    WORKING_DIRECTORY ${ARGV0}/build)
endfunction()

function(set_paths)
    target_include_directories(${ARGV1} PUBLIC ${ARGV0}/build/include)
    target_link_directories(${ARGV1} PUBLIC ${ARGV0}/build/lib)
    target_link_libraries(${ARGV1} GLEW)
endfunction()

