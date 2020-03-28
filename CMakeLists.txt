cmake_minimum_required(VERSION 3.2)
project(engsci-press C)

include_directories(src)

file(GLOB CORE_SOURCE src/core/*.c)
add_library(core STATIC ${CORE_SOURCE})

file(GLOB TEST_SOURCE src/test/*.c)
foreach(source_name ${TEST_SOURCE})
    get_filename_component(executable_name ${source_name} NAME_WE)
    add_executable(${executable_name} ${source_name})
    target_link_libraries(${executable_name} core)
endforeach(source_name ${TEST_SOURCE})