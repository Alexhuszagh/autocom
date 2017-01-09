#   :copyright: (c) 2015-2016 The Regents of the University of California.
#   :license: MIT, see LICENSE.md for more details.
#
#   \brief This script generates header import files for the COM DLL.
#
#   Requires:
#       The generate tool requires a pre-built AutoCOM executable, and
#       this must be "found" by CMake, setting `AutoCOM_FOUND`,
#       `AutoCOM_CONFIGURE_EXECUTABLE`, and `AutoCOM_INCLUDE_DIRS`.
#
#   SourceFile:
#       The source file should have `#include` directives with:
#           `#inclue "@AUTOCOM:progid=WScript.Shell.1:namespace=wsh@"
#
#       Required fileds:
#           progid --       ProgID or CLSID for COM object
#
#       Optional Fields:
#           namespace --    Valid namespace identifier for import
#
#   \use
#       AutoCOMConfigure(source "/path/to/source.cpp")
#

if (NOT AutoCOM_FOUND)
    return()
endif()


# Get CLSID from ProgID for import header.
#
#   Args:
#       output  -- Name of variable to store result in
#       progid  -- ProgID for COM DLL
#
#   Example:
#       GetCLSID(clsid "WScript.Shell.1")
#
function(GetCLSID output progid)
    execute_process(COMMAND ${AutoCOM_CONFIGURE_EXECUTABLE} -progid=${progid} -mode=clsid
        OUTPUT_VARIABLE clsid
        RESULT_VARIABLE status
    )

    if (${status})
        message(SEND_ERROR "Unable to get CLSID for import, error code ${status}")
    endif()

    set(${output} ${clsid} PARENT_SCOPE)
endfunction(GetCLSID)


# Get CLSID from ProgID for import header.
#
#   Args:
#       progid  -- ProgID for COM DLL
#
#   Example:
#       ConfigureCom("WScript.Shell.1" "")
#
function(ConfigureCom progid namespace)
    execute_process(COMMAND ${AutoCOM_CONFIGURE_EXECUTABLE} -progid=${progid} -ns=${namespace}
        RESULT_VARIABLE status
    )
    if (${status})
        message(SEND_ERROR "Unable to get generate header for COM DLL, error code ${status}")
    endif()
endfunction(ConfigureCom)


# Configure source file, generate import headers for COM building.
#
#   Args:
#       output -- Name of variable to store configured file in
#
#   Example:
#       AutoCOMConfigure(/path/to/source.cpp)
#

function(AutoCOMConfigure output source)
    include_directories("${CMAKE_CURRENT_BINARY_DIR}")
    include_directories(${AutoCOM_INCLUDE_DIRS})

    # PATTERNS
    set(progid_re ".*:progid=([{}A-Za-z.0-9]+)[:@](.*)")
    set(namespace_re ".*:namespace=([A-Za-z_0-9]*)[:@].*")

    # GET ATTRIBUTES
    file(READ ${source} contents)
    string(REGEX MATCHALL "#include[\\t ]+\"@AUTOCOM:.*progid=.+@\"" items ${contents})
    foreach(item ${items})
        # PROGID/Namespace
        set(namespace "")
        STRING(REGEX REPLACE ${progid_re} "\\1" progid "${item}")
        if(item MATCHES ${namespace_re})
            STRING(REGEX REPLACE ${namespace_re} "\\1" namespace "${item}")
        endif()

        # CLSID
        GetCLSID(clsid ${progid})
        ConfigureCom("${progid}" "${namespace}")

        string(REPLACE "${item}" "#include \"${clsid}.hpp\"" contents "${contents}")
    endforeach()

    # WRITE
    get_filename_component(OUTPUT_NAME ${source} NAME)
    set(configured "${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_NAME}")
    file(WRITE ${configured} "${contents}")

    # STORE
    set(${output} ${configured} PARENT_SCOPE)
endfunction(AutoCOMConfigure)
