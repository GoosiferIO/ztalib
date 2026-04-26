# `PalF` Class

The `PalF` class is the main class for parsing and working with Zoo Tycoon 1 palette files.

Source code in `include/ztalib/PalF.h` and `src/PalF.cpp`.

## Import

Import the `PalF` class as so:

```c++
#include "ztalib/PalF.h"
```

## `class` `PalF`

```c++
class PalF
{
    struct Color 
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };
    PalF();
    
    int load(std::string fileName);
    void save(std::string fileName);
    std::string location();
    void location(std::string loc);

    uint32_t locationSize();
    void locationSize(uint32_t size);

    int colorModel();
    void colorModel(int model);

    std::vector<Color> colors() const;
    void colors(const std::vector<Color>& newColors);

    Color getColor(int index);
};
```

### Example

=== "C++"
    ```c++
    #include "ztalib/PalF.h"

    int main()
    {
        PalF pal;
    }
    ```
=== "Python"
    ```python
    from pyzta import PalF

    pal = PalF()
    ```

### `struct` `PalF::Color`

```c++
struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};
```

Represents a single color in the palette. Contains red, green, blue, and alpha components as 8-bit unsigned integers.

### `meth` `load`

```c++
int load(std::string fileName);
```

Load a PalF file from disk. Returns 0 on success, non-zero on failure.

**Example**

=== "C++"
    ```c++
    #include "ztalib/PalF.h"

    int main()
    {
        PalF pal;
        int result = pal.load("path/to/palette");
        if (result != 0) {
            // Handle error
        }
    }
    ```
=== "Python"
    ```python
    from pyzta import PalF

    pal = PalF()
    result = pal.load("path/to/palette")
    if result != 0:
        # Handle error
    ```

### `meth` `save`

```c++
void save(std::string fileName);
```

Save the current palette data to a PalF file on disk.

**Example**
=== "C++"
    ```c++
    #include "ztalib/PalF.h"

    int main()
    {
        PalF pal;
        // Assume palette is loaded and modified
        pal.save("path/to/new_palette");
    }
    ```
=== "Python"
    ```python
    from pyzta import PalF
    pal = PalF()
    # Assume palette is loaded and modified
    pal.save("path/to/new_palette")
    ```

### `meth` `location`

```c++
std::string location();
void location(std::string loc);
```

Get or set the location of the palette file. This is the file path used when loading the palette.

**Example**

=== "C++"
    ```c++
    #include "ztalib/PalF.h"

    int main()
    {
        PalF pal;
        pal.location("path/to/palette");
        std::string loc = pal.location();

        std::cout << "Palette location: " << loc << std::endl;
    }
    ```
=== "Python"
    ```python
    from pyzta import PalF

    pal = PalF()
    pal.location("path/to/palette")
    loc = pal.location()

    print(f"Palette location: {loc}")
    ```

### `meth` `locationSize`

```c++
uint32_t locationSize();
void locationSize(uint32_t size);
```

Get or set the size of the palette file. This is the number of bytes in the file. Byte count must be accurate for future encoding of the palette file to work correctly.

**Example**

=== "C++"
    ```c++
    #include "ztalib/PalF.h"

    int main()
    {
        PalF pal;
        pal.locationSize(1024);
        uint32_t size = pal.locationSize();

        std::cout << "Palette size: " << size << std::endl;
    }
    ```
=== "Python"
    ```python
    from pyzta import PalF

    pal = PalF()
    pal.locationSize(1024)
    size = pal.locationSize()

    print(f"Palette size: {size}")
    ```

### `meth` `colorModel`

```c++
int colorModel();
void colorModel(int model);
```

Get or set the color model used when parsing the palette file. 0 == RGBA, 1 == BGRA.

**Example**

=== "C++"
    ```c++
    #include "ztalib/PalF.h"

    int main()
    {
        PalF pal;
        pal.colorModel(0); // Set to RGBA
        int model = pal.colorModel();

        std::cout << "Color model: " << (model == 0 ? "RGBA" : "BGRA") << std::endl;
    }
    ```
=== "Python"
    ```python
    from pyzta import PalF

    pal = PalF()
    pal.colorModel(0) # Set to RGBA
    model = pal.colorModel()

    print(f"Color model: {'RGBA' if model == 0 else 'BGRA'}")
    ```

### `meth` `colors`

```c++
std::vector<Color> colors() const;
void colors(const std::vector<Color>& newColors);
```

Get or set the colors in the palette as a vector of `Color` structs.

**Example**

=== "C++"
    ```c++
    #include "ztalib/PalF.h"

    int main()
    {
        PalF pal;
        std::vector<PalF::Color> newColors = {
            {255, 0, 0, 255},   // Red
            {0, 255, 0, 255},   // Green
            {0, 0, 255, 255}    // Blue
        };
        pal.colors(newColors);

        std::vector<PalF::Color> colors = pal.colors();
        for (size_t i = 0; i < colors.size(); ++i) {
            const auto& color = colors[i];
            std::cout << "Color " << i << " - R: " << (int)color.r 
                      << " G: " << (int)color.g 
                      << " B: " << (int)color.b 
                      << " A: " << (int)color.a << std::endl;
        }
    }
    ```
=== "Python"
    ```python
    from pyzta import PalF

    pal = PalF()
    new_colors = [
        PalF.Color(255, 0, 0, 255),   # Red
        PalF.Color(0, 255, 0, 255),   # Green
        PalF.Color(0, 0, 255, 255)    # Blue
    ]
    pal.colors(new_colors)

    colors = pal.colors()
    for i, color in enumerate(colors):
        print(f"Color {i} - R: {color.r} G: {color.g} B: {color.b} A: {color.a}")
    ```

### `meth` `getColor`

```c++
Color getColor(int index);
```

Get the color at the specified index in the palette. Returns a `Color` struct containing the RGBA values of the color.

**Example**

=== "C++"
    ```c++
    #include "ztalib/PalF.h"

    int main()
    {
        PalF pal;
        // Assume palette is loaded and has colors
        PalF::Color color = pal.getColor(0); // Get the first color

        std::cout << "Color 0 - R: " << (int)color.r 
                  << " G: " << (int)color.g 
                  << " B: " << (int)color.b 
                  << " A: " << (int)color.a << std::endl;
    }
    ```
=== "Python"
    ```python
    from pyzta import PalF

    pal = PalF()
    # Assume palette is loaded and has colors
    color = pal.getColor(0) # Get the first color

    print(f"Color 0 - R: {color.r} G: {color.g} B: {color.b} A: {color.a}")
    ```