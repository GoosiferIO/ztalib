cmake_minimum_required(VERSION 3.15...4.1)

project(ztalib 
    VERSION 0.7.0
    LANGUAGES CXX
)

add_library(ztalib SHARED 
    src/data/ZtaData.cpp
    src/data/ZtaFrameBuffer.cpp
    src/parsers/ZtaF.cpp
    src/parsers/PalF.cpp
)

add_subdirectory(src/data)
add_subdirectory(src/parsers)

target_include_directories(ztalib 
    PUBLIC 
        $include/ztalib)