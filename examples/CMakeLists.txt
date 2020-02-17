cmake_minimum_required(VERSION 3.5)
set (CMAKE_CXX_STANDARD 11)
project(attysexamples)

add_executable (attysdemo attysdemo.cpp)
target_link_libraries(attysdemo attyscomm_static pthread)
target_include_directories(attysdemo PRIVATE ..)
