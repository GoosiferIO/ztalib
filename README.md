# ztalib

This is an image parsing library for Zoo Tycoon 1 animation graphics.

For complete documentation: https://ztalib.goosifer.io

# Install

The easiest way to start using ztalib is to use the Python bindings, which are available on PyPI as `pyzta`:

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

See [the example project](https://ztalib.goosifer.io/example/pngexporter/) to see it in action, or check out the [API reference](https://ztalib.goosifer.io/reference/ztaf/) for more details on how to use the library.

## Compile from source

If you want to use the library in a C++ project, or if you want to contribute to the development of ztalib, you can compile it from source. First, make sure you have CMake installed on your system. For further instructions, see the [compile guide](https://ztalib.goosifer.io/compile/).

# Acknowledgements

Thank you to Jeff Bostoen for his documentation on the ZT1 file formats:
https://github.com/jbostoen/ZTStudio/wiki/ZT1-Graphics-Explained
