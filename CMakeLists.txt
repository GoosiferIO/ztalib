cmake_minimum_required(VERSION 3.15...4.1)

# define project
project(ztalib 
    VERSION 0.7.0
    LANGUAGES CXX
)

# add all source files to library
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

# add include dirs
target_include_directories(stb_image_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/libs 
)

# link libraries
target_link_libraries(ztalib PUBLIC 
    stb_image_lib
)