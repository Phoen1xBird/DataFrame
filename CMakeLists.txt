project(DataFrame)
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

aux_source_directory(MY_SOURCES src)

add_executable(DataFrame
    src/main.cpp

)

target_include_directories(DataFrame PUBLIC include)
target_compile_options(DataFrame PUBLIC -g)
