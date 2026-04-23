cmake_minimum_required(VERSION 3.15...4.1)

# define project
project(ztalib 
    VERSION 0.7.0
    LANGUAGES CXX
)

# add all source files to library
add_library(ztalib SHARED 
    # data
    src/data/ZtaData.cpp
    src/data/ZtaFrameBuffer.cpp
    # other
    src/other/ZtaUtils.cpp
    # parsers
    src/parsers/ZtaF.cpp
    src/parsers/PalF.cpp
)