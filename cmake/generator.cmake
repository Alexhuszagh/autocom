#   :copyright: (c) 2015-2016 The Regents of the University of California.
#   :license: MIT, see licenses/mit.md for more details.
#
#   \brief This script generates header import files for the COM DLL.
#   \use
#       AutoComGenerate("MSScriptControl.ScriptControl" "${CMAKE_CURRENT_BINARY_DIR}" MSScriptControl)
#
#


macro(AutoComGenerate ProgID Directory Target)

#    # Get PROGID
#    execute_process(COMMAND $<TARGET_FILE:AutoCOMProgID> -progid=${ProgID}
#        DEPENDS AutoCOMProgID
#        WORKING_DIRECTORY ${Directory}
#        OUTPUT_VARIABLE ImportID
#        RESULT_VARIABLE Status
#    )
#
#    # Get CLSID
#    execute_process(COMMAND $<TARGET_FILE:AutoCOMCLSID> -progid=${ProgID}
#        DEPENDS AutoCOMCLSID
#        WORKING_DIRECTORY ${Directory}
#        OUTPUT_VARIABLE CLSID
#        RESULT_VARIABLE Status
#    )

    # TODO: need to get the progid from AutoCOMProgID
    # TODO: need to get the clsid from AutoCOMCLSID
    add_custom_target(${Target}
        COMMAND $<TARGET_FILE:AutoCOMGenerator> -progid=${ProgID}
        DEPENDS AutoCOMGenerator)

endmacro(AutoComGenerate)
