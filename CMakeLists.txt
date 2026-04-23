cmake_minimum_required(VERSION 3.15...4.1)

# define project
project(ztalib 
    VERSION 0.7.0
    LANGUAGES CXX
)

file(GLOB SOURCE_FILES "src/*.cpp" "src/*.h")

# add all source files to library
add_library(ztalib SHARED 
    # data
    src/data/ZtaColor.h
    src/data/ZtaData.cpp src/data/ZtaData.h
    src/data/ZtaFrame.h 
    src/data/ZtaFrameBuffer.cpp src/data/ZtaFrameBuffer.h
    src/data/ZtaInfo.h 
    src/data/ZtaPixelBlock.h
    src/data/ZtaPixelSet.h 
    # other
    src/other/ZtaUtils.cpp src/other/ZtaUtils.h
    # parsers
    src/parsers/ZtaF.cpp src/parsers/ZtaF.h
    src/parsers/PalF.cpp src/parsers/PalF.h
)

# add include dirs
target_include_directories(ztalib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/libs 
)

# link libraries
target_link_libraries(ztalib PUBLIC 
    stb_image_lib
)