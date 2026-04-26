# ztalib

ztalib is a parsing library for Zoo Tycoon 1 animation graphics and palette files.

## Repository

https://github.com/GoosiferIO/ztalib

## Features

- Read Zoo Tycoon 1 animation graphics and palette files in-memory
- Supports most animated graphics in the game
- CMake build system for easy integration into C++ projects

# Install

The easiest way to start using ztalib is to use the Python bindings, which are available on PyPI:

```bash
pip install pyzta
```

Then, in your Python code, you can import the library as so:

```python
from pyzta import ZtaF
zta = ZtaF()
animation = zta.load("path/to/animation")
buffer = zta.getFrameBuffer()
```

See [the example project](./example/pngexporter.md) to see it in action, or check out the [API reference](./reference/ztaf.md) for more details on how to use the library.

## Compile from source

If you want to use the library in a C++ project, or if you want to contribute to the development of ztalib, you can compile it from source. First, make sure you have CMake installed on your system. For further instructions, see the [compile guide](./compile.md).