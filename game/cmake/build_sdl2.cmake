cmake_minimum_required(VERSION 3.10)

function(prepare)
    execute_process(COMMAND git clone https://github.com/libsdl-org/SDL.git ${ARGV0})
    execute_process(COMMAND mkdir build
                    WORKING_DIRECTORY ${ARGV0})
endfunction()

function(build)
    execute_process(COMMAND git fetch
                    WORKING_DIRECTORY ${ARGV0})

    execute_process(COMMAND git reset --hard 4cd981609b50ed273d80c635c1ca4c1e5518fb21
                    WORKING_DIRECTORY ${ARGV0})

    execute_process(COMMAND cmake ..
                    WORKING_DIRECTORY ${ARGV0}/build)

    execute_process(COMMAND make
                    WORKING_DIRECTORY ${ARGV0}/build)
endfunction()

function(set_paths)
    target_include_directories(${ARGV1} PUBLIC ${ARGV0}/include ${ARGV0}/build/include)
    target_link_directories(${ARGV1} PUBLIC ${ARGV0}/build/sdl2)
    target_link_libraries(${ARGV1} SDL2 SDL2main)
endfunction()
