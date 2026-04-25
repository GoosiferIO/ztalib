# `ZtaF` Class

The `ZtaF` class is the main class for parsing and working with Zoo Tycoon 1 animation graphic files. 

## Import

Import the `ZtaF` class as so:

```c++
#include "ztalib/ZtaF.h"
```

## `class` `ZtaF`

```c++
class ZtaF
{
public:
    ZtaF();
    virtual ~ZtaF();

    std::shared_ptr<ZtaData> load(
        std::string fileName, 
        int colorProfile = 0, 
        std::string ioPal = ""
    );
    std::shared_ptr<ZtaData> data();
    std::vector<ZtaFrameBufferObject> getFrameBuffer();

private:
    std::vector<ZtaFrameBufferObject> m_frameBuffer;
    std::shared_ptr<ZtaData> m_data;
    int m_colorModel;
    int hasMagic(std::ifstream &);
};
```

### Example

```c++
#include "ztalib/ZtaF.h"

int main()
{
    ZtaF zta;
}
```

### `meth` `ZtaF()`