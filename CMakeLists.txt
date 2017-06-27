#  :copyright: (c) 2015-2016 The Regents of the University of California.
#  :license: MIT, see LICENSE.md for more details.

set(CMAKE_LEGACY_CYGWIN_WIN32 1)
cmake_minimum_required(VERSION 2.8)
project(autocom CXX)
enable_testing()

# FLAGS
# -----

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# OPTIONS
# -------

if(NOT WIN32)
    message(FATAL_ERROR "COM interface only works on Windows")
endif()

option(BUILD_EXAMPLES "Build example files" ON)
option(BUILD_EXECUTABLE "Build AutoCOM executable" ON)
option(BUILD_STATIC "Build static library" ON)
option(BUILD_TESTS "Build unittests (requires GTest)" OFF)
option(HAVE_THERMO "Have Thermo MSFileReader for examples" OFF)
option(HAVE_SCRIPTCONTROL "Have MSScriptControl for examples" OFF)

if(BUILD_STATIC)
    if (MINGW)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    elseif(MSVC)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd /O0 /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /Ox /Zi /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
        set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT /Zi /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
    endif()
endif()

# Need to manually set compiler flags for Travis CI
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

# DEPENDENCIES
# ------------

add_subdirectory(funxx)
include_directories(funxx/src)

# LIBRARY
# -------

set(AUTOCOM_SOURCES
    src/util/alias.cpp
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
    add_library(autocom STATIC ${AUTOCOM_SOURCES})
else()
    add_library(autocom SHARED ${AUTOCOM_SOURCES})
endif()

set(AUTOCOM_LIBRARIES autocom funxx)
if(MSVC)
    list(APPEND AUTOCOM_LIBRARIES ole32.lib oleaut32.lib uuid.lib)
elseif(MINGW OR MSYS)
    list(APPEND AUTOCOM_LIBRARIES -lole32 -loleaut32 -luuid)
endif()

# BIN
# ----

set(AUTOCOM_EXECUTABLE_SOURCES
    bin/autocom.cpp
    bin/options.cpp
    bin/parse.cpp
    bin/write.cpp
)

if(BUILD_EXECUTABLE)
    if(NOT TARGET gflags)
        add_subdirectory(gflags)
    endif()
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/gflags/include")
    add_executable(autocomconf ${AUTOCOM_EXECUTABLE_SOURCES})
    target_link_libraries(autocomconf ${AUTOCOM_LIBRARIES} gflags)
endif()

# EXAMPLES
# --------

set(AUTOCOM_LATEBINDING_EXAMPLE_SOURCES
    example/late/regexp.cpp
    example/late/thermo.cpp
    example/late/wsh.cpp
)

# LATE
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

    foreach(source ${AUTOCOM_LATEBINDING_EXAMPLE_SOURCES})
        get_filename_component(example ${source} NAME_WE)
        set(target "${example}_laye")
        add_executable(${target} "${source}")
        target_link_libraries(${target} ${AUTOCOM_LIBRARIES})
    endforeach(source)
endif()

# EARLY
if (BUILD_EXAMPLES AND BUILD_EXECUTABLE)
    include_directories("${CMAKE_CURRENT_BINARY_DIR}")

    # WScript
    add_custom_target(wscript
        COMMAND $<TARGET_FILE:autocomconf> -progid="WScript.Shell.1"
        DEPENDS autocomconf)
    add_executable(wsh_early example/early/wsh.cpp)
    target_link_libraries(wsh_early ${AUTOCOM_LIBRARIES})
    add_dependencies(wsh_early "wscript")

    # MSScriptControl
    if(HAVE_SCRIPTCONTROL)
        add_custom_target(msscriptcontrol
            COMMAND $<TARGET_FILE:autocomconf> -progid="MSScriptControl.ScriptControl"
            DEPENDS autocomconf)
        add_executable(script_early example/early/script.cpp)
        target_link_libraries(script_early ${AUTOCOM_LIBRARIES})
        add_dependencies(script_early "msscriptcontrol")
    endif()

    # Thermo
    if (HAVE_THERMO)
        add_custom_target(msfilereader
            COMMAND $<TARGET_FILE:autocomconf> -progid="{1D23188D-53FE-4C25-B032-DC70ACDBDC02}"
            DEPENDS autocomconf)
        add_executable(thermo_early example/early/thermo.cpp)
        target_link_libraries(thermo_early ${AUTOCOM_LIBRARIES})
        add_dependencies(thermo_early "msfilereader")
    endif()
endif()

# TESTS
# -----

set(AUTOCOM_TEST_SOURCES
    test/bin/parse.cpp
    test/src/util/alias.cpp
    test/src/util/type.cpp
    test/src/bstr.cpp
    test/src/dispparams.cpp
    test/src/guid.cpp
    test/src/safearray.cpp
    test/src/variant.cpp
    test/src/main.cpp

    # GENERATOR
    bin/parse.cpp
    bin/write.cpp
)

if (BUILD_TESTS)
    if(NOT TARGET gtest)
        add_subdirectory(googletest)
    endif()
    include_directories(googletest/googletest/include bin)

    add_executable(autocom_tests ${AUTOCOM_TEST_SOURCES})
    target_link_libraries(autocom_tests
        gtest
        gtest_main
        ${AUTOCOM_LIBRARIES}
    )

    # Need to specify proper exception handling for Windows.
    if(MSVC)
        set_target_properties(autocom_tests PROPERTIES
            COMPILE_OPTIONS "/EHsc"
        )
    endif()

    add_test(NAME autocom_tests
        COMMAND autocom_tests
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

endif()
