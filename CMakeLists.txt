cmake_minimum_required(VERSION 3.16)

project(mygamesavechange CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(MY_DIR_PATH ${PROJECT_SOURCE_DIR})

function(my_set_common_properties target)

    # set_target_properties(${target} PROPERTIES 
    #                         CMAKE_CXX_STANDARD 20
    #                         CMAKE_CXX_STANDARD_REQUIRED ON)
  
    target_compile_definitions(${target} PUBLIC
        _WIN32_WINNT=0x0602
        NTDDI_VERSION=0x06020000
    )
   
    target_compile_options(${target} PUBLIC
        -pedantic-errors
        -Wsign-conversion
        -Woverflow
        -Wconversion
        -Werror
        -Wextra
        -fconcepts
    )

    target_link_options(${target} PUBLIC
    -static)
   
    target_include_directories(${target} PUBLIC
        ${MY_DIR_PATH}/include
    )


    target_link_libraries(${target} PUBLIC
    )

   

endfunction()


function(my_set_target_out targetname)
    # 设置输出目录
    set_target_properties(${targetname} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${MY_DIR_PATH}/out
    OUTPUT_NAME "${targetname}"
    )
endfunction()


add_executable(savegamechange ./savegamechange.cpp)


my_set_common_properties(savegamechange)


my_set_target_out(savegamechange)


add_executable(trimbitspan ./trimbitspan.cpp)


my_set_common_properties(trimbitspan)


my_set_target_out(trimbitspan)
