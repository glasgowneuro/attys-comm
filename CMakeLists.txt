cmake_minimum_required(VERSION 3.5)
set (CMAKE_CXX_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE "Release")
endif()

project(attyscomm VERSION 2.0.7 LANGUAGES CXX)
include(GNUInstallDirs)
add_subdirectory(test)
add_subdirectory(examples)

if(APPLE)
set(LIBSRC
  AttysComm.mm
  AttysCommBase.cpp
  AttysScan.mm
  attyscomm/base64.cpp)
elseif(NOT APPLE)
set(LIBSRC
  AttysComm.cpp
  AttysCommBase.cpp
  AttysScan.cpp
  attyscomm/base64.cpp)
endif(APPLE)

set(PUBLICHEADERS
  AttysComm.h
  AttysCommBase.h
  AttysScan.h)

set(PRIVATEHEADERS
  attyscomm/base64.h)

if(APPLE)

set(ios_frameworks
  CoreBluetooth
  IOBluetooth
  Foundation
)

foreach (FRAMEWORK_NAME ${ios_frameworks})
  list(APPEND linked_ios_frameworks "-framework ${FRAMEWORK_NAME}")
endforeach (FRAMEWORK_NAME)

endif(APPLE)

if(UNIX)
  
  add_library(attyscomm
    SHARED
    ${LIBSRC}
    )
  
  target_include_directories(attyscomm PRIVATE attyscomm)
  
  set_target_properties(attyscomm PROPERTIES
    SOVERSION 2
    VERSION ${PROJECT_VERSION}
    PUBLIC_HEADER "${PUBLICHEADERS}"
    PRIVATE_HEADER "${PRIVATEHEADERS}")
  
  install(TARGETS attyscomm
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    PRIVATE_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/attyscomm)

  if(APPLE)
    target_link_libraries(attyscomm ${linked_ios_frameworks})
  endif(APPLE)
  
endif(UNIX)

add_library(attyscomm_static
  STATIC
  ${LIBSRC}
  )

if(WIN32)
  target_link_libraries(attyscomm_static ws2_32)
endif(WIN32)

if(APPLE)
    target_link_libraries(attyscomm_static ${linked_ios_frameworks})
endif(APPLE)

target_include_directories(attyscomm_static PRIVATE attyscomm)

set_target_properties(attyscomm_static PROPERTIES
  VERSION ${PROJECT_VERSION}
  PUBLIC_HEADER "${PUBLICHEADERS}"
  PRIVATE_HEADER "${PRIVATEHEADERS}")

install(TARGETS attyscomm_static
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  PRIVATE_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/attyscomm)

