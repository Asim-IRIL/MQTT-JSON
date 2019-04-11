# CMakeLists.txt
#
# Top-level CMake file for the Paho C++ library.
#
#*******************************************************************************
# This is part of the Paho MQTT C++ client library.
#
# Copyright (c) 2016-2017, Guilherme Maciel Ferreira
# Copyright (c) 2017, Frank Pagliughi
#
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v1.0
# and Eclipse Distribution License v1.0 which accompany this distribution.
# 
# The Eclipse Public License is available at
#   http://www.eclipse.org/legal/epl-v10.html
# and the Eclipse Distribution License is available at
#   http://www.eclipse.org/org/documents/edl-v10.php.
# 
# Contributors:
#   Guilherme Maciel Ferreira - initial version
#   Frank Pagliughi
#*******************************************************************************/

## Note: on OS X you should install XCode and the associated command-line tools

## cmake flags
cmake_minimum_required(VERSION 3.5)

## project name
project("paho-mqtt-cpp" 
    VERSION "1.0.1"
    LANGUAGES CXX
)

## --- Build options ---

if(WIN32)
  option(PAHO_BUILD_STATIC "Build static library" TRUE)
  option(PAHO_BUILD_SHARED "Build shared library (DLL)" FALSE)
  option(PAHO_WITH_SSL "Build SSL-enabled library" FALSE)
else()
  option(PAHO_BUILD_STATIC "Build static library" FALSE)
  option(PAHO_BUILD_SHARED "Build shared library" TRUE)
  option(PAHO_WITH_SSL "Build SSL-enabled library" TRUE)
endif()

option(PAHO_BUILD_SAMPLES "Build sample programs" FALSE)
option(PAHO_BUILD_TESTS "Build tests" FALSE)
option(PAHO_BUILD_DOCUMENTATION "Create and install the API documentation (requires Doxygen)" FALSE)

## --- C++11 build flags ---

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- System Libraries ---

include(GNUInstallDirs)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set(LIBS_SYSTEM ws2_32)
elseif(UNIX)
    set(LIBS_SYSTEM c stdc++)
endif()

## --- Build directories ---

# For the paho_mqtt_c module
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
add_subdirectory(src)

# --- Documentation ---

if(PAHO_BUILD_DOCUMENTATION)
    add_subdirectory(doc)
endif()

# --- Default library for samples and unit tests ---

if(PAHO_BUILD_SHARED)
    set(PAHO_CPP_LIB paho-mqttpp3)
else()
    set(PAHO_CPP_LIB paho-mqttpp3-static)
endif()

# --- Sample Apps ---

if(PAHO_BUILD_SAMPLES)
    add_subdirectory(src/samples)
endif()

# --- Unit Tests ---

if(PAHO_BUILD_TESTS)
    add_subdirectory(test/unit)
endif()

## --- Packaging settings ---

if(WIN32)
    set(CPACK_GENERATOR "ZIP")
elseif(UNIX)
    set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)
add_subdirectory(cmake)

# Typically you don't care so much for a third party library's tests to be
# run from your own project's code.
set(JSON_BuildTests OFF CACHE INTERNAL "")

# If you only include this third party in PRIVATE source files, you do not
# need to install it when your main project gets installed.
# set(JSON_Install OFF CACHE INTERNAL "")

# Don't use include(nlohmann_json/CMakeLists.txt) since that carries with it
# unintended consequences that will break the build.  It's generally
# discouraged (although not necessarily well documented as such) to use
# include(...) for pulling in other CMake projects anyways.

if(FOO_USE_EXTERNAL_JSON)
  find_package(nlohmann_json 3.2.0 REQUIRED)
else()
  set(JSON_BuildTests OFF CACHE INTERNAL "")
  add_subdirectory(nlohmann_json)
endif()

#add_subdirectory(nlohmann_json)
#add_library(foo ...)
#target_link_libraries(foo PRIVATE nlohmann_json::nlohmann_json)