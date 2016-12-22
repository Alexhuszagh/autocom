#  :copyright: (c) 2015-2016 The Regents of the University of California.
#  :license: BSD, see LICENSE.md for more details.

cmake_minimum_required(VERSION 2.8)
project(Autocom CXX)

# FLAGS
# -----

set(CMAKE_CXX_STANDARD 11)

# OPTIONS
# -------

if(NOT WIN32)
    message(FATAL_ERROR "COM interface only works on Windows")
endif()

option(BUILD_EXAMPLES "Build example files" ON)
option(BUILD_STATIC "Build static library" ON)
option(BUILD_TESTS "Build unittests (requires GTest)" OFF)

if(MINGW AND ${BUILD_STATIC})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
endif()

# LIBRARY
# -------

set(AUTOCOM_SOURCES
    src/encoding/converters.cpp
    src/encoding/unicode.cpp
    src/dispparams.cpp
    src/guid.cpp
)

set(AUTOCOM_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(AUTOCOM_INCLUDE_DIRS ${AUTOCOM_INCLUDE_DIR})
include_directories(${AUTOCOM_INCLUDE_DIRS})

if(BUILD_STATIC)
    add_library(Autocom STATIC ${AUTOCOM_SOURCES})
else()
    add_library(Autocom SHARED ${AUTOCOM_SOURCES})
endif()

set_target_properties(Autocom PROPERTIES OUTPUT_NAME autocom)
set(AUTOCOM_LIBRARIES Autocom)
if(MSVC)
    list(APPEND AUTOCOM_LIBRARIES ole32.lib oleaut32.lib uuid.lib)
elseif(MINGW)
    list(APPEND AUTOCOM_LIBRARIES -lole32 -loleaut32 -luuid)
endif()

# EXAMPLES
# --------

# TESTS
# -----

set(AUTOCOM_TEST_SOURCES
    test/encoding/converters.cpp
    test/encoding/unicode.cpp
    test/dispparams.cpp
    test/guid.cpp
    test/main.cpp
)

if (BUILD_TESTS)
     add_subdirectory(googletest)
     add_executable(AutoComTests ${AUTOCOM_TEST_SOURCES})
     target_link_libraries(AutoComTests
        gtest
        gtest_main
        ${AUTOCOM_LIBRARIES}
    )

     add_custom_target(check
        COMMAND $<TARGET_FILE:AutoComTests>
        DEPENDS AutoComTests
    )

endif()
