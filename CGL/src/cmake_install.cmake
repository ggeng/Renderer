# Install script for directory: /Users/George/cs184/asst2_geomenagerie/CGL/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/George/cs184/asst2_geomenagerie/build/CGL/src/libCGL.a")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCGL.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCGL.a")
    execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCGL.a")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/CGL" TYPE FILE FILES
    "/Users/George/cs184/asst2_geomenagerie/CGL/src/CGL.h"
    "/Users/George/cs184/asst2_geomenagerie/CGL/src/vector2D.h"
    "/Users/George/cs184/asst2_geomenagerie/CGL/src/vector3D.h"
    "/Users/George/cs184/asst2_geomenagerie/CGL/src/vector4D.h"
    "/Users/George/cs184/asst2_geomenagerie/CGL/src/matrix3x3.h"
    "/Users/George/cs184/asst2_geomenagerie/CGL/src/matrix4x4.h"
    "/Users/George/cs184/asst2_geomenagerie/CGL/src/quaternion.h"
    "/Users/George/cs184/asst2_geomenagerie/CGL/src/complex.h"
    "/Users/George/cs184/asst2_geomenagerie/CGL/src/color.h"
    "/Users/George/cs184/asst2_geomenagerie/CGL/src/osdtext.h"
    "/Users/George/cs184/asst2_geomenagerie/CGL/src/viewer.h"
    "/Users/George/cs184/asst2_geomenagerie/CGL/src/base64.h"
    "/Users/George/cs184/asst2_geomenagerie/CGL/src/tinyxml2.h"
    "/Users/George/cs184/asst2_geomenagerie/CGL/src/renderer.h"
    )
endif()

