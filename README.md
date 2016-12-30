AutoCom
=======

AutoCom is a C++11 interface for the Component Object Model (COM) supporting MinGW and MSVC, inspired by [ComTypes](https://github.com/enthought/comtypes).

**Table of Contents**

- [Motivation](#motivation)
- [Interface](#interface)
- [Reference](#reference)
- [Building](#building)
- [Issues](#issues)
- [Contributors](#contributors)
- [License](#license)

## Motivation

AutoCom is a modern COM interface library for C++11. No more manually dispatching parameters by hand.
// TODO: finish motivation, do this later

## Interface

AutoCom supports both early and late-binding interfaces, for both compile-time interface wrapping and dynamic dispatchers.

**Early-binding**

```

```

**Late-binding**

```cpp
#include "autocom.hpp"
#include <cstdio>


int main(int argc, char *argv[])
{
    com::Dispatch dispatch("VBScript.RegExp");
    dispatch.put("Pattern", L"\\w+");
    dispatch.put("IgnoreCase", TRUE);
    dispatch.put("Global", TRUE);

    // get matches
    INT index, length;
    com::Bstr text;
    for (auto match: dispatch.iter("Execute", L"A(b) c35 d_[x] yyy")) {
        match.get("FirstIndex", index);
        match.get("Length", length);
        match.get("Value", text);
        printf("Match found from %d-%d and is '%S'\n", index, index+length, text.string);
    }

    return 0;
}
```

## Reference

// TODO:

## Building

Simply clone, configure with CMake, and build.

```bash
git clone https://github.com/Alexhuszagh/autocom.git
# git submodule update --init   # Uncomment to build tests
cd autocom/build
cmake ..                        # `-DBUILD_EXAMPLES=ON`, `-DBUILD_TESTS=ON`
make -j 5                       # "msbuild autocom.sln" for MSVC
```

## Issues

Due to the automated memory management and argument forwarding of AutoCom, unexpected behavior can arise (with buggy code).

-- Using uninitialized pointers, especially BSTRS, with `Dispatch::method` or `Dispatch::put` methods (use `autocom::Bstr` instead).

## Contributors

- Alex Huszagh

## License

MIT, see [license](LICENSE.md).
