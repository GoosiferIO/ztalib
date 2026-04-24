# ztalib

This is an image parsing library for Zoo Tycoon 1 animation graphics.

## Compiling

### C++ Library

Easiest way to start using the library is compile it as a C++ library. You can use CMake:

```bash
cmake -S . -B build
cmake --build build --config Release
cmake --install build --config Release --prefix dist
```

This will install the library in a `dist` folder. Copy the contents of `dist` to your project and link against the library.

