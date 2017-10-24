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
option(BUILD_TESTS "Build unittests (requires GTest)" OFF)
option(HAVE_THERMO "Have Thermo MSFileReader for examples" OFF)
option(HAVE_SCRIPTCONTROL "Have MSScriptControl for examples" OFF)

if(NOT BUILD_SHARED_LIBS)
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

# LIBRARY
# -------

add_subdirectory(third_party/pycpp)

set(AUTOCOM_SOURCES
    src/util/alias.cc
    src/util/exception.cc
    src/util/type.cc
    src/bstr.cc
    src/com.cc
    src/dispparams.cc
    src/dispatch.cc
    src/enum.cc
    src/iterator.cc
    src/guid.cc
    src/safearray.cc
    src/typeinfo.cc
    src/variant.cc
)

set(AUTOCOM_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(AUTOCOM_INCLUDE_DIRS ${AUTOCOM_INCLUDE_DIR})

set(AUTOCOM_LINK_LIBRARIES pycpp)
if(MSVC)
    list(APPEND AUTOCOM_LINK_LIBRARIES ole32.lib oleaut32.lib uuid.lib)
elseif(MINGW OR MSYS)
    list(APPEND AUTOCOM_LINK_LIBRARIES -lole32 -loleaut32 -luuid)
endif()

add_library(autocom ${AUTOCOM_SOURCES})
target_include_directories(autocom PUBLIC ${AUTOCOM_INCLUDE_DIRS})
target_link_libraries(autocom LINK_PUBLIC ${AUTOCOM_LINK_LIBRARIES})

# BIN
# ----

set(AUTOCOM_EXECUTABLE_SOURCES
    bin/autocom.cc
    bin/options.cc
    bin/parse.cc
    bin/write.cc
)

if(BUILD_EXECUTABLE)
    if(NOT TARGET gflags)
        add_subdirectory(third_party/gflags)
    endif()
    add_executable(autocomconf ${AUTOCOM_EXECUTABLE_SOURCES})
    target_link_libraries(autocomconf PUBLIC autocom gflags)
    target_include_directories(autocomconf PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/third_party/gflags/include")
endif()

# EXAMPLES
# --------

set(AUTOCOM_LATEBINDING_EXAMPLE_SOURCES
    example/late/regexp.cc
    example/late/thermo.cc
    example/late/wsh.cc
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
        target_link_libraries(${target} PUBLIC autocom)
    endforeach(source)
endif()

# EARLY
if (BUILD_EXAMPLES AND BUILD_EXECUTABLE)

    # WScript
    add_custom_target(wscript
        COMMAND $<TARGET_FILE:autocomconf> -progid="WScript.Shell.1"
        DEPENDS autocomconf)
    add_executable(wsh_early example/early/wsh.cc)
    target_link_libraries(wsh_early PUBLIC autocom)
    target_include_directories(wsh_early PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
    add_dependencies(wsh_early "wscript")

    # MSScriptControl
    if(HAVE_SCRIPTCONTROL)
        add_custom_target(msscriptcontrol
            COMMAND $<TARGET_FILE:autocomconf> -progid="MSScriptControl.ScriptControl"
            DEPENDS autocomconf)
        add_executable(script_early example/early/script.cc)
        target_link_libraries(script_early PUBLIC autocom)
        target_include_directories(script_early PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
        add_dependencies(script_early "msscriptcontrol")
    endif()

    # Thermo
    if (HAVE_THERMO)
        add_custom_target(msfilereader
            COMMAND $<TARGET_FILE:autocomconf> -progid="{1D23188D-53FE-4C25-B032-DC70ACDBDC02}"
            DEPENDS autocomconf)
        add_executable(thermo_early example/early/thermo.cc)
        target_link_libraries(thermo_early PUBLIC autocom)
        target_include_directories(thermo_early PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
        add_dependencies(thermo_early "msfilereader")
    endif()
endif()

# TESTS
# -----

set(AUTOCOM_TEST_SOURCES
    test/bin/parse.cc
    test/src/util/alias.cc
    test/src/util/type.cc
    test/src/bstr.cc
    test/src/dispparams.cc
    test/src/guid.cc
    test/src/safearray.cc
    test/src/variant.cc
    test/src/main.cc

    # GENERATOR
    bin/parse.cc
    bin/write.cc
)

if (BUILD_TESTS)
    if(NOT TARGET gtest)
        add_subdirectory(third_party/googletest)
    endif()

    add_executable(autocom_tests ${AUTOCOM_TEST_SOURCES})
    target_include_directories(autocom_tests PRIVATE googletest/googletest/include bin)
    target_link_libraries(autocom_tests
        gtest
        gtest_main
        autocom
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
