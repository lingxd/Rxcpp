# Copyright 2017 MongoDB Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Demonstrates how to use the CMake 'find_package' mechanism to locate build against the libmongocxx
# static library.

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

if(POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif()

project(HELLO_WORLD LANGUAGES C CXX)

# Enforce the C++ standard, and disable extensions.
if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

# NOTE: For this to work, the CMAKE_PREFIX_PATH variable must be set to point to the directory that
# was used as the argument to CMAKE_INSTALL_PREFIX when building libmongocxx.
# find_package(xxx REQUIRED)

# We cannot rely on the DYLD_LIBRARY_PATH being propagated properly on OSX on
# evergreen, so pass extra arguments to pkg-config with the path information.
if (APPLE)
    set(ENV{PKGCONFIG_EXTRA_OPTS} "-Wl,rpath $(pwd)/../mongoc/lib")
endif()

include_directories(
    include
)

# set(PROJECT_EXAMPLES
#     src/First.cpp
#     src/Second.cpp
#     src/STLContainerStream.cpp
#     src/ObserverFromScratch.cpp
#     src/Concat.cpp
# )

file(GLOB PROJECT_EXAMPLES "src/*.cpp")

foreach(SRC ${PROJECT_EXAMPLES})
    get_filename_component(EXAMPLE_TARGET ${SRC} NAME_WE)
    add_executable(${EXAMPLE_TARGET} ${SRC})
    target_include_directories(${EXAMPLE_TARGET} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
    # Visual Studio pre 2017 requires boost polyfill.
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND CMAKE_CXX_STANDARD LESS 17)
    find_package(Boost 1.56.0 REQUIRED)
    if (CMAKE_VERSION VERSION_LESS 3.15.0)
        target_include_directories(hello_example PRIVATE ${Boost_INCLUDE_DIRS})
    else()
        target_link_libraries(hello_example PRIVATE Boost::boost)
    endif()
    endif()

    target_link_libraries(${EXAMPLE_TARGET}
        pthread
        # PRIVATE mongo::mongocxx_shared
    )
endforeach(SRC)

add_custom_target(run
    COMMAND hello_example
    DEPENDS hello_example
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

# Sanity-check that static library macros are not set when building against the shared library.
# Users don't need to include this section in their projects.
# get_target_property(LIBMONGOCXX_DEFINITIONS mongo::mongocxx_shared INTERFACE_COMPILE_DEFINITIONS)
# list(FIND LIBMONGOCXX_DEFINITIONS "BSONCXX_STATIC" LIST_IDX)
# if (${LIST_IDX} GREATER -1)
#     message(FATAL_ERROR "Expected BSONCXX_STATIC to not be defined")
# endif()
# list(FIND LIBMONGOCXX_DEFINITIONS "MONGOCXX_STATIC" LIST_IDX)
# if (${LIST_IDX} GREATER -1)
#     message(FATAL_ERROR "Expected MONGOCXX_STATIC to not be defined")
# endif()