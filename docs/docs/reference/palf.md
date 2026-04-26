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

```c++
#include "ztalib/PalF.h"

int main()
{
    PalF pal;
    int result = pal.load("path/to/file");
    if (result != 0) {
        // Handle error
    }
}
```

### `meth` `location`

```c++
std::string location();
void location(std::string loc);
```

Get or set the location of the palette file. This is the file path used when loading the palette.

**Example**

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

### `meth` `locationSize`

```c++
uint32_t locationSize();
void locationSize(uint32_t size);
```

Get or set the size of the palette file. This is the number of bytes in the file. Byte count must be accurate for future encoding of the palette file to work correctly.

**Example**

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

### `meth` `colorModel`

```c++
int colorModel();
void colorModel(int model);
```

Get or set the color model used when parsing the palette file. 0 == RGBA, 1 == BGRA.

**Example**

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

### `meth` `colors`

```c++
std::vector<Color> colors() const;
void colors(const std::vector<Color>& newColors);
```

Get or set the colors in the palette as a vector of `Color` structs.

**Example**

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

### `meth` `getColor`

```c++
Color getColor(int index);
```

Get the color at the specified index in the palette. Returns a `Color` struct containing the RGBA values of the color.

**Example**

```c++
#include "ztalib/PalF.h"

int main()
{
    PalF pal;
    // Assume palette is loaded and has colors
    Color color = pal.getColor(0); // Get the first color

    std::cout << "Color 0 - R: " << (int)color.r 
              << " G: " << (int)color.g 
              << " B: " << (int)color.b 
              << " A: " << (int)color.a << std::endl;
}
```