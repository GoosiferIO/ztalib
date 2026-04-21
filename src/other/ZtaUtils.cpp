#include "ZtaUtils.h"

int ZtaUtils::hasMagic(std::ifstream &_file)
{
    char magic[5] = {0};
    _file.read(magic, 4);

    // std::cout << "\tMagic Bytes: " << magic << std::endl;

    // read at least 4 bytes
    // if less than 4 bytes, not FATZ
    if (_file.gcount() < 4)
    {
        _file.clear();
        _file.seekg(0, std::ios::beg);
        return 0;
    }

    // test for FATZ
    if (strcmp(magic, MAGIC) != 0)
    {
        _file.clear();
        _file.seekg(0, std::ios::beg);
        return 0;
    }

    // FATZ found
    _file.clear();
    _file.seekg(0, std::ios::beg);
    return 1;
}
