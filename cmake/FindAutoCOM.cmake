#   :copyright: (c) 2015-2016 The Regents of the University of California.
#   :license: MIT, see LICENSE.md for more details.

# FindAutoCOM
# -----------
#
# Find AutoCOM include dirs, libraries, and executables.
#
# Use this module by invoking find_package with the form:
#
#   find_package(AutoCOM
#     [REQUIRED]             # Fail with error if AutoCOM is not found
#     )
#
# If found, `AutoCOM_FOUND` will be set to true, and `AutoCOM_LIBRARIES`,
# `AutoCOM_INCLUDE_DIRS`, and `AutoCOM_EXECUTABLE` will be set.

# REPEAT
# ------

if (AutoCOM_FOUND)
    return()
endif()

# FIND
# ----

FIND_PATH(AutoCOM_INCLUDE_DIRS autocom.hpp)
FIND_PROGRAM(AutoCOM_EXECUTABLE
    NAMES autocomconf
    PATH_SUFFIXES bin
    DOC "The autocom executable for the AutoCOM to use"
)
FIND_LIBRARY(AutoCOM_LIBRARIES
    NAMES autocom
    DOC "AutoCOM library path"
)

if(MSVC)
    list(APPEND AutoCOM_LIBRARIES ole32.lib oleaut32.lib uuid.lib)
else()
    list(APPEND AutoCOM_LIBRARIES -lole32 -loleaut32 -luuid)
endif()

if (AutoCOM_INCLUDE_DIRS AND AutoCOM_EXECUTABLE AND AutoCOM_LIBRARIES)
    set(AutoCOM_FOUND TRUE)
endif()

if(AutoCOM_FOUND)
    message("Found AutoCOM.")
elseif(AutoCOM_FIND_REQUIRED)
    message(SEND_ERROR "Unable to find AutoCOM.")
endif()
