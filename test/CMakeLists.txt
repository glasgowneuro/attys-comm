cmake_minimum_required(VERSION 3.5)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE "Debug")

add_executable (attystest attystest.cpp)
target_include_directories(attystest PRIVATE ..)

if (UNIX)
  if(APPLE)
    target_link_libraries(attystest attyscomm_static pthread)
  elseif(NOT APPLE)
    target_link_libraries(attystest attyscomm_static pthread bluetooth)
  endif(APPLE)
endif(UNIX)

if(WIN32)
  target_link_libraries(attystest attyscomm_static ws2_32)
endif(WIN32)



add_executable (scantest scantest.cpp)
target_include_directories(scantest PRIVATE ..)

if (UNIX)
  if(APPLE)
    target_link_libraries(scantest attyscomm_static pthread)
  elseif(NOT APPLE)
    target_link_libraries(scantest attyscomm_static pthread bluetooth)
  endif(APPLE)
endif(UNIX)

if(WIN32)
  target_link_libraries(scantest attyscomm_static ws2_32)
endif(WIN32)
