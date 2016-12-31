## AutoCom

-- Need to create compile-time wrappers around methods that use COM automation with an IDispatch interface
    -- Similar to ComTypes, only create the actual interface **at** compilation.

- Create a TLB parser
    - This can be a basic C++ method, needs to export a C++ class
        - Need basic structs, as well as virtual dispatchers.


## TODO
    -- Define open methods
        -- RAII needs to be exception-proof, so define open methods.

    -- TODO: Use Vtbl offsets
        -- https://stackoverflow.com/questions/9983967/vtables-and-function-pointers-pointing-to-different-addresses
    -- The static IIDs aren't working...
        -- Need to be "{...-...-}", not "..._..._"

    -- TODO: need to create derived classes from dispatchers
        -- Manage co-initialization
        -- Read description here... 
            https://gist.github.com/markddavidoff/4f0f22e225d0520cfe18
        -- Implement IClassFactory and IClassFactory2 
            https://msdn.microsoft.com/en-us/library/windows/desktop/ms694364(v=vs.85).aspx

        -- Need a safearray wrapper and this needs to be tested...
            http://stackoverflow.com/a/12484259/4131059
        -- Need to test unions... -- DONE

## OLD
-- First need to test my methods.... Shit.


-- TODO: need to find out how to get the structures...
    -- generate_module(tlib, ofi, pathname) -- DONE
    -- Equality operation for tlib... -- DONE
    -- TODO:

        def _register(self, name, value, tlib=None):
        -- modname = tlib.GetLibAttr()
        -- fullname = "%s.%s" % (modname, name)
        -- self.items[fullname] = value
            -- Ok, they're storing the sring of the raw TlibAttrInfo
                -- TLIBATTR(GUID={F0C5F3E3-4F2A-443E-A74D-0AABE3237494}, Version=1.0, LCID=0, FLags=0x8).MS_DataPeak
                -- I just need the name...

    -- Use PDB
        -- pdb.runcall(test.foo, 1, 2)


Compile Methods...
    (<class 'ctypes.HRESULT'>, 'Open', (<class 'comtypes.BSTR'>,), ((0, 'szFileName'),), (1, u'method Open'), u'method Open')
    WINFUNCTYPE(ctypes.HRESULT, comtypes.BSTR)

    restype, name, argtypes, paramflags, idlflags, doc = item
    _IXRawfile__com_Open

    paramflags = ((0, 'szFileName'),)


http://thrysoee.dk/InsideCOM+/ch05d.htm
http://www.drdobbs.com/writing-com-clients-with-late-and-early/184403558
