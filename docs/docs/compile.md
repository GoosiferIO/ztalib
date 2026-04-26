# Compile

## C++ Library

To compile ztalib as a shared C++ library, go to the root of the repository and run:

=== "Windows/MSVC"
    ```bash
    cmake -S . -B build
    cmake --build build --config Release
    cmake --install build --config Release --prefix dist
    ```
=== "Windows/MinGW"
    ```bash
    cmake -G "MinGW Makefiles" -S . -B build
    cmake --build build --config Release
    cmake --install build --config Release --prefix dist
    ```
=== "Linux/macOS"
    ```bash
    cmake -S . -B build
    cmake --build build --config Release
    cmake --install build --config Release --prefix dist
    ```

This will install the library in a `/dist` folder. Copy the contents of `/dist` to your project and link against the library.

## Python Bindings

For Python projects that want to use ztalib, bindings are also supported. First, make sure you have a virtual environment activated if you do not want to install the ztalib bindings system-wide:

=== "Windows/PowerShell"
    ```ps
    python -m venv venv
    venv/Scripts/Activate.ps1
    ```
=== "Windows/Command Prompt"
    ```
    python -m venv venv
    venv\Scripts\activate.bat
    ```
=== "Linux/macOS"
    ```
    python -m venv venv
    source venv/bin/activate
    ```

Then, in the root of the repository, run:

=== "Windows/MSVC"
    ```bash
    cmake -S . -B build -DBUILD_PY_BINDINGS=ON
    cmake --build build --config Release
    cmake --install build --config Release
    ```
=== "Windows/MinGW"
    ```bash
    cmake -G "MinGW Makefiles" -S . -B build -DBUILD_PY_BINDINGS=ON
    cmake --build build --config Release
    cmake --install build --config Release
    ```
=== "Linux/macOS"
    ```bash
    cmake -S . -B build -DBUILD_PY_BINDINGS=ON
    cmake --build build --config Release
    cmake --install build --config Release
    ```

This will install the Python bindings to the currently-active Python environment. You can then import the `pyzta` module in your Python code:

```python
import pyzta
```

# Setup

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

Then, in your C++ code, you can include the headers as so:

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

See [the example project](./docs/example.md) to see it in action.