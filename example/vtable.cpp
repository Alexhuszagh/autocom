//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief Example to use test virtual table offset methods.
 */

#include "autocom.hpp"

#include <tchar.h>

#include <cassert>
#include <cstdio>
#include <iostream>

namespace com = autocom;
//
//MS_MAX_MS_ORDER = 100
//__MIDL___MIDL_itf_XRawfile2_0000_0000_0004 = c_int # enum
//class IXRawfile(comtypes.gen._00020430_0000_0000_C000_000000000046_0_2_0.IDispatch):
//    _case_insensitive_ = True
//    u'IXRawfile Interface'
//    _iid_ = GUID('{11B488A0-69B1-41FC-A660-FE8DF2A31F5B}')
//    _idlflags_ = ['dual', 'oleautomation']




struct IXVirUV: IDispatch
{
};


struct IXRawfile: IDispatch
{
    virtual HRESULT STDMETHODCALLTYPE Open(BSTR szFileName);
    virtual HRESULT STDMETHODCALLTYPE Close();
    virtual HRESULT STDMETHODCALLTYPE GetFileName(BSTR *name);
    virtual HRESULT STDMETHODCALLTYPE GetCreatorID(BSTR *creator);
    virtual HRESULT STDMETHODCALLTYPE GetVersionNumber(LONG *version);
};


struct IXRawfile2: IXRawfile
{};



struct IXRawfile3: IXRawfile2
{};


struct IXRawfile4: IXRawfile3
{};


//struct IXRawfile5: IXRawfile4
//{};




/** \brief Execute main code block.
 */
int main(int argc, char *argv[])
{
    GUID msraw, ixrviruv, IID_IXRaw, IID_IXRaw2, IID_IXRaw3, IID_IXRaw4, IID_IXRaw5, IID_VIRUV;
    IUnknown *xraw, *xviruv;
    IDispatch *dispatch;
    IXVirUV *ixviruv;
    IXRawfile *ixraw;
    IXRawfile2 *ixraw2;
    IXRawfile3 *ixraw3;
    IXRawfile4 *ixraw4;
    CLSIDFromString(L"{1D23188D-53FE-4C25-B032-DC70ACDBDC02}", reinterpret_cast<LPCLSID>(&msraw));
    IIDFromString(L"{11B488A0-69B1-41FC-A660-FE8DF2A31F5B}", reinterpret_cast<LPIID>(&IID_IXRaw));
    IIDFromString(L"{55A25FF7-F437-471F-909A-D7F2B5930805}", reinterpret_cast<LPIID>(&IID_IXRaw2));
    IIDFromString(L"{19A00B1E-1559-42B1-9A46-08A5E599EDEE}", reinterpret_cast<LPIID>(&IID_IXRaw3));
    IIDFromString(L"{E7CF6760-11CD-4260-B5B0-FCE2AD97547B}", reinterpret_cast<LPIID>(&IID_IXRaw4));
    IIDFromString(L"{06F53853-E43C-4F30-9E5F-D1B3668F0C3C}", reinterpret_cast<LPIID>(&IID_IXRaw5));

    CLSIDFromString(L"{B3DB6431-A183-43F1-8BBD-F354064D9041}", reinterpret_cast<LPCLSID>(&ixrviruv));
    IIDFromString(L"{796CB3FE-C696-4AFE-B719-18246F38A740}", reinterpret_cast<LPIID>(&IID_VIRUV));

    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(CoCreateInstance(msraw, nullptr, CLSCTX_INPROC_SERVER, IID_IUnknown, (void **) &xraw))) {
        assert(false);
    }
    if (FAILED(CoCreateInstance(ixrviruv, nullptr, CLSCTX_INPROC_SERVER, IID_IUnknown, (void **) &xviruv))) {
        assert(false);
    }

    if (FAILED(xviruv->QueryInterface(IID_VIRUV, (void**)&ixviruv))) {
        assert(false);
    }

    if (FAILED(xraw->QueryInterface(IID_IDispatch, (void**)&dispatch))) {
        assert(false);
    }
    dispatch->Release();

    if (FAILED(xraw->QueryInterface(IID_IXRaw4, (void**)&ixraw4))) {
        assert(false);
    } else {
        BSTR szPathName = SysAllocString(L"Thermo.raw");
        std::cout << "Open() -> " << ixraw4->Open(szPathName) << std::endl;
//        LONG version;
//        std::cout << "GetVersionNumber() -> " << ixraw4->GetVersionNumber(&version) << std::endl;


    }
    ixraw4->Release();

    //CLSIDFromProgID(L"MSXML2.XMLHTTP", reinterpret_cast<LPCLSID>(&guid));
    //CoCreateInstance(guid, nullptr, CLSCTX_INPROC_SERVER, IID_IDispatch, (void **) &dispatch);

    //HRESULT (IUnknown::*pointer_to_function)(REFIID, void**);
    //typedef HRESULT (IUnknown::*ptr)(REFIID, void**);
    //pointer_to_function = &IDispatch::QueryInterface;
    // QueryInterface
    // TODO:
    // auto tlib = dispatch.info().typelib();
    // for (UINT index = 0; index < tlib.count(); ++index) {
    //     auto info = tlib.info(index);
    //     auto attr = info.attr();
    //     if (attr.kind() == TKIND_INTERFACE) {
    //         for (WORD index = 0; index < attr.functions(); ++index) {
    //             auto fd = info.funcdesc(index);
    //             std::cout << info.documentation(fd.id()).name << "\n";
    //             // fd.offset() + ;
    //         }
    //     }
    // }

    CoUninitialize();

    return 0;
}
