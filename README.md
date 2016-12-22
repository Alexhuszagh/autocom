AutoCom
=======

Lattice is a C++11 interface for the Component Object Model (COM), inspired by [ComTypes](https://github.com/enthought/comtypes.

**Table of Contents**

- [Motivation](#motivation)
- [Building](#building)
- [Contributors](#contributors)
- [License](#license)

## Motivation

AutoCom is a modern COM interface library for C++11. No more manually dispatching parameters by hand.
// TODO: finish motivation, do this later

## Building

Simply clone, configure with CMake, and build.

```bash
git clone https://github.com/Alexhuszagh/autocom.git
# git submodule update --init   # Uncomment to build tests
cd autocom/build
cmake ..                        # `-DBUILD_EXAMPLES=ON`, `-DBUILD_TESTS=ON`
make -j 5                       # "msbuild autocom.sln" for MSVC
```

## Contributors

- Alex Huszagh

## License

MIT, see [license](LICENSE.md).
