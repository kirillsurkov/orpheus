include(CheckRetcode)

cmake_minimum_required(VERSION 3.10)

function(prepare)
    execute_process(COMMAND git clone https://github.com/Reputeless/PerlinNoise ${ARGV0}
                    RESULT_VARIABLE retcode)
    check_retcode()
endfunction()

function(build)
    execute_process(COMMAND git fetch
                    RESULT_VARIABLE retcode
                    WORKING_DIRECTORY ${ARGV0})
    check_retcode()

    execute_process(COMMAND git reset --hard aea505706d6caaa3f845c9478fafaa7f7330f7b8
                    RESULT_VARIABLE retcode
                    WORKING_DIRECTORY ${ARGV0})
    check_retcode()
endfunction()

function(set_paths)
    target_include_directories(${ARGV1} PUBLIC ${ARGV0})
endfunction()
