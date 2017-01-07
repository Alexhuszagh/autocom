## AutoCom

-- Need to create compile-time wrappers around methods that use COM automation with an IDispatch interface
    -- Similar to ComTypes, only create the actual interface **at** compilation.

## TODO: 
    - Overload setVariant and getVariant to use SAFEARRAY -- DONE
    - Overload PutVariant and GetVariant for get/setVariant -- DONE
    - Change getVariant to get -- DONE
    - Change setVariant to set -- DONE
    - Need to test VariantType -- DONE
    - Overload Get/PutSafeArray -- DONE
    - Test Get/PutVariant -- DONE
    - Overload SafeArray for get/set -- DONE
    - Finish STL wrapper for SAFEARRAY -- DONE
    - Test SafeArray -- DONE
    - TODO: need to change everything to have native WinAPI properties as *backups* -- DONE

    -- TODO: need to get the DLL to link against from:
        GetModuleFileName
        DllEntryPoint

        -- TODO: bug with arrays

    -- TODO: (this is wrong)
         __stdcall HRESULT GetMassListFromScanNum 
     -- TODO: This is right
        virtual HRESULT STDMETHODCALLTYPE Open(BSTR szFileName);



## TODO
    -- Define open methods
        -- RAII needs to be exception-proof, so define open methods.

    -- TODO: need to create derived classes from dispatchers
        -- Manage co-initialization
        -- Read description here... 
            https://gist.github.com/markddavidoff/4f0f22e225d0520cfe18
        -- Implement IClassFactory and IClassFactory2 
            https://msdn.microsoft.com/en-us/library/windows/desktop/ms694364(v=vs.85).aspx

        -- Need to test unions... -- DONE

http://thrysoee.dk/InsideCOM+/ch05d.htm
http://www.drdobbs.com/writing-com-clients-with-late-and-early/184403558
