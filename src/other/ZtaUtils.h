#ifndef ZTAUTILS_H
#define ZTAUTILS_H

#include <fstream>
#include <cstring>

#define MAGIC "FATZ"

class ZtaUtils
{
public:
    static int hasMagic(std::ifstream &);
};

#endif // ZTAUTILS_H