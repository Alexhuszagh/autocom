#  :copyright: (c) 2015-2016 The Regents of the University of California.
#  :license: BSD, see LICENSE.md for more details.

cmake_minimum_required(VERSION 2.8)
project(AutoCOM CXX)

# FLAGS
# -----

set(CMAKE_CXX_STANDARD 14)

# OPTIONS
# -------

if(NOT WIN32)
    message(FATAL_ERROR "COM interface only works on Windows")
endif()

option(BUILD_EXAMPLES "Build example files" ON)
option(BUILD_GENERATOR "Build early-binding generator" ON)
option(BUILD_STATIC "Build static library" ON)
option(BUILD_TESTS "Build unittests (requires GTest)" OFF)

if(BUILD_STATIC)
    if (MINGW)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    elseif(MSVC)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_RELEASE} /MTd /O0")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /Ox")
    endif()
endif()

# LIBRARY
# -------

set(AUTOCOM_SOURCES
    src/encoding/converters.cpp
    src/encoding/unicode.cpp
    src/util/exception.cpp
    src/util/type.cpp
    src/bstr.cpp
    src/com.cpp
    src/dispparams.cpp
    src/dispatch.cpp
    src/enum.cpp
    src/iterator.cpp
    src/guid.cpp
    src/safearray.cpp
    src/typeinfo.cpp
    src/variant.cpp
)

set(AUTOCOM_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(AUTOCOM_INCLUDE_DIRS ${AUTOCOM_INCLUDE_DIR})
include_directories(${AUTOCOM_INCLUDE_DIRS})

if(BUILD_STATIC)
    add_library(AutoCOM STATIC ${AUTOCOM_SOURCES})
else()
    add_library(AutoCOM SHARED ${AUTOCOM_SOURCES})
endif()

set_target_properties(AutoCOM PROPERTIES OUTPUT_NAME autocom)
set(AUTOCOM_LIBRARIES AutoCOM)
if(MSVC)
    list(APPEND AUTOCOM_LIBRARIES ole32.lib oleaut32.lib uuid.lib)
elseif(MINGW)
    list(APPEND AUTOCOM_LIBRARIES -lole32 -loleaut32 -luuid)
endif()

# BIN
# ----

set(AUTOCOM_GENERATOR_SOURCES
    bin/generator.cpp
    bin/parser.cpp
    bin/writer.cpp
)

# MSXML2.XMLHTTP implements GetTypeInfo

if(BUILD_GENERATOR)
    add_subdirectory(gflags)
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/gflags/include")
    add_executable(AutoCOMGenerator ${AUTOCOM_GENERATOR_SOURCES})
    set_target_properties(AutoCOMGenerator PROPERTIES OUTPUT_NAME autocom_generator)
    target_link_libraries(AutoCOMGenerator ${AUTOCOM_LIBRARIES} gflags)
endif()

# EXAMPLES
# --------

set(AUTOCOM_EXAMPLE_SOURCES
    example/regexp.cpp
    example/thermo.cpp
    example/vtable.cpp
)

if (BUILD_EXAMPLES)
    set(OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
    if(MSVC)
        if(CMAKE_BUILD_TYPE MATCHES Release)
            set(OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY}/Release")
        else()
            set(OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY}/Debug")
        endif()
    endif()

    file(COPY example/Thermo.raw
        DESTINATION ${OUTPUT_DIRECTORY}
    )

    foreach(source ${AUTOCOM_EXAMPLE_SOURCES})
        get_filename_component(example ${source} NAME_WE)
        set(target "AutoCOM_${example}")
        add_executable(${target} "${source}")
        set_target_properties(${target} PROPERTIES OUTPUT_NAME ${example})
        target_link_libraries(${target} ${AUTOCOM_LIBRARIES})
    endforeach(source)
endif()

# TESTS
# -----

set(AUTOCOM_TEST_SOURCES
    test/bin/parser.cpp
    test/src/encoding/converters.cpp
    test/src/encoding/unicode.cpp
    test/src/util/type.cpp
    test/src/bstr.cpp
    test/src/dispparams.cpp
    test/src/guid.cpp
    test/src/safearray.cpp
    test/src/variant.cpp
    test/src/main.cpp

    # GENERATOR
    bin/parser.cpp
    bin/writer.cpp
)

if (BUILD_TESTS)
     add_subdirectory(googletest)
     include_directories("${CMAKE_CURRENT_SOURCE_DIR}/bin")
     add_executable(AutoCOMTests ${AUTOCOM_TEST_SOURCES})
     target_link_libraries(AutoCOMTests
        gtest
        gtest_main
        ${AUTOCOM_LIBRARIES}
    )

     add_custom_target(check
        COMMAND $<TARGET_FILE:AutoCOMTests>
        DEPENDS AutoCOMTests
    )

endif()
