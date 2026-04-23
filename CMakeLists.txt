cmake_minimum_version(3.15...4.1)

project(ztalib 
    VERSION 0.7.0
    LANGUAGES CXX
)

add_library(ztalib shared 
    # data
    src/ZtaColor.h
    data/ZtaData.cpp src/ZtaData.h
    data/ZtaFrame.h 
    data/ZtaFrameBuffer.cpp src/ZtaFrameBuffer.h
    data/ZtaInfo.h 
    data/ZtaPixelBlock.h
    data/ZtaPixelSet.h 
    # other
    other/ZtaUtils.cpp src/ZtaUtils.h
    # parsers
    parsers/ZtaF.cpp src/parsers/ZtaF.h
    parsers/PalF.cpp src/parsers/PalF.h
)