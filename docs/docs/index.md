# ztalib

ztalib is a parsing library for Zoo Tycoon 1 animation graphics and palette files.

## Features

- Read Zoo Tycoon 1 animation graphics and palette files in-memory
- Supports most animated graphics in the game
- CMake build system for easy integration into C++ projects

## Compile

### C++ Library

Easiest way to start using ztalib is to install it as a C++ library. In the root of the repository, run:

```bash
cmake -S . -B build
cmake --build build --config Release
cmake --install build --config Release --prefix dist
```

This will install the library in a `/dist` folder. Copy the contents of `/dist` to your project and link against the library.