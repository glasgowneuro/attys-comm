cmake_minimum_required(VERSION 3.5)
set (CMAKE_CXX_STANDARD 11)
project(attysexamples)
set(CMAKE_BUILD_TYPE "Debug")

add_executable (attysdemo attysdemo.cpp)

if(WIN32)
  target_link_libraries(attysdemo attyscomm_static ws2_32)
endif(WIN32)

if (UNIX)
  if(APPLE)
    target_link_libraries(attysdemo attyscomm_static pthread)
  elseif(NOT APPLE)
    target_link_libraries(attysdemo attyscomm_static pthread bluetooth)
  endif(APPLE)
endif(UNIX)

target_include_directories(attysdemo PRIVATE ..)
