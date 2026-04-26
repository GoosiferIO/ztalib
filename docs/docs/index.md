# ztalib

ztalib is a parsing library for Zoo Tycoon 1 animation graphics and palette files.

## Repository

https://github.com/GoosiferIO/ztalib

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

### Python Bindings

Python bindings are available as an option. To build the Python bindings, run:

```bash
cmake -S . -B build -BUILD_PY_BINDINGS=ON
cmake --build build --config Release
cmake --install build --config Release --prefix dist
```

## Setup

Copy the contents of `/dist` to your project and link against the library.

For example, a possible project structure could be:

```
MyProject/
├── include/
│   └── ztalib/
│       ├── PalF.h
│       └── AniF.h
├── lib/
│   └── libzta.lib
├── bin/
│   └── libzta.dll
├── src/
│   └── main.cpp
```

Then, in your C++ code, you can include the headers and link against the library:

```c++
#include "ztalib/ZtaF.h"

int main()
{
    ZtaF zta;

    auto animation = zta.load("path/to/animation");
    auto buffer = zta.getFrameBuffer();

    std::cout << "Animation loaded with " << buffer.size() << " frames!" << std::endl;
}
```