# `ZtaF` Class

The `ZtaF` class is the main class for parsing and working with Zoo Tycoon 1 animation graphic files. 

Source code in `include/ztalib/ZtaF.h` and `src/ZtaF.cpp`.

## Import

Import the `ZtaF` class as so:

=== "C++"
    ```c++
    #include "ztalib/ZtaF.h"
    ```
=== "Python"
    ```python
    from pyzta import ZtaF
    ```

## `class` `ZtaF`

```c++
class ZtaF
{
    ZtaF();
    std::shared_ptr<ZtaData> load(
        std::string fileName, 
        int colorProfile = 0, 
        std::string ioPal = ""
    );
    std::shared_ptr<ZtaData> data();
    std::vector<ZtaFrameBufferObject> getFrameBuffer();
};
```

### Example

=== "C++"
    ```c++
    #include "ztalib/ZtaF.h"

    int main()
    {
        ZtaF zta;
    }
    ```
=== "Python"
    ```python
    from pyzta import ZtaF

    zta = ZtaF()
    ```

### `meth` `load`

```c++
    std::shared_ptr<ZtaData> load(
        std::string fileName, 
        int colorProfile = 0, 
        std::string ioPal = ""
    );
```

Load a ZtaF file from disk. Returns a `std::shared_ptr` to a `ZtaData` object containing the parsed data.

**Example**

=== "C++"
    ```c++
    #include "ztalib/ZtaF.h"

    int main()
    {
        ZtaF zta;
        auto data = zta.load("path/to/file");
    }
    ```

    | Parameter | Description |
    |--- | --- |
    | `fileName` | The path to the ZtaF file to load. |
    | `colorProfile` | (Optional) The color profile to use when parsing the file. 0 == RGBA, 1 == BGRA |
    | `ioPal` | (Optional) The path to a PalF file to use for color information. If not provided, the parser will attempt to extract the correct palette from the ZtaF file. |
=== "Python"
    ```python
    from pyzta import ZtaF

    zta = ZtaF()
    data = zta.load("path/to/file")
    ```

    | Parameter | Description |
    |--- | --- |
    | `file_name` | The path to the ZtaF file to load. |
    | `color_profile` | (Optional) The color profile to use when parsing the file. 0 == RGBA, 1 == BGRA |
    | `io_pal` | (Optional) The path to a PalF file to use for color information. If not provided, the parser will attempt to extract the correct palette from the ZtaF file. |

### `meth` `data`

```c++
    std::shared_ptr<ZtaData> data();
```

Returns a `std::shared_ptr` to the `ZtaData` object containing the parsed data. This can be used after calling `load` to access the parsed data.

**Example**

=== "C++"
    ```c++
    #include "ztalib/ZtaF.h"

    int main()
    {
        ZtaF zta;
        zta.load("path/to/file");
        auto data = zta.data();
    }
    ```
=== "Python"
    ```python
    from pyzta import ZtaF

    zta = ZtaF()
    zta.load("path/to/file")
    data = zta.data()
    ```

### `meth` `getFrameBuffer`

```c++
    std::vector<ZtaFrameBufferObject> getFrameBuffer();
```

Returns a `std::vector` of `ZtaFrameBufferObject`s containing the frame buffer data for each frame in the animation. This is the primary buffer that can be used for rendering the animation frames to compatible graphics APIs.

**Example**

=== "C++"
    ```c++
    #include "ztalib/ZtaF.h"

    int main()
    {
        ZtaF zta;
        zta.load("path/to/file");
        auto frameBuffer = zta.getFrameBuffer();
    }
    ```
=== "Python"
    ```python
    from pyzta import ZtaF

    zta = ZtaF()
    zta.load("path/to/file")
    frame_buffer = zta.getFrameBuffer()
    ```