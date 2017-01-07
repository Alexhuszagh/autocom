#   :copyright: (c) 2015-2016 The Regents of the University of California.
#   :license: MIT, see licenses/mit.md for more details.
#
#   \brief This script generates header import files for the COM DLL.
#   \use
#       AutoComGenerate("MSScriptControl.ScriptControl" "${CMAKE_CURRENT_BINARY_DIR}" MSScriptControl)
#
#


macro(AutoComGenerate ProgID Directory Target)

    add_custom_target(${Target}
        COMMAND $<TARGET_FILE:AutoCOMGenerator> -progid=${ProgID}
        DEPENDS AutoCOMGenerator)

endmacro(AutoComGenerate)
