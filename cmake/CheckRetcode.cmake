cmake_minimum_required(VERSION 3.10)

function(check_retcode)
    if (DEFINED retcode AND NOT ${retcode} EQUAL 0)
        message(FATAL_ERROR "Process returned ${retcode}")
    endif()
endfunction()
