#ifndef APEUTILS_H
#define APEUTILS_H

#include <fstream>
#include <cstring>

#define MAGIC "FATZ"

class ApeUtils
{
public:
    static int hasMagic(std::ifstream &);
};

#endif // APEUTILS_H