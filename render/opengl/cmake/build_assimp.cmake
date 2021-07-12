cmake_minimum_required(VERSION 3.10)

function(prepare)
    execute_process(COMMAND git clone https://github.com/assimp/assimp.git ${ARGV0})
    execute_process(COMMAND mkdir build
                    WORKING_DIRECTORY ${ARGV0})
endfunction()

function(build)
    execute_process(COMMAND git fetch
                    WORKING_DIRECTORY ${ARGV0})

    execute_process(COMMAND git reset --hard 8f0c6b04b2257a520aaab38421b2e090204b69df
                    WORKING_DIRECTORY ${ARGV0})

    execute_process(COMMAND cmake ..
                    WORKING_DIRECTORY ${ARGV0}/build)

    execute_process(COMMAND make
                    WORKING_DIRECTORY ${ARGV0}/build)
endfunction()

function(set_paths)
    target_include_directories(${ARGV1} PUBLIC ${ARGV0}/build/include)
    target_link_directories(${ARGV1} PUBLIC ${ARGV0}/build/code)
    target_link_libraries(${ARGV1} assimp)
endfunction()
