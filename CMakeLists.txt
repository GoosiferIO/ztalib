cmake_minimum_required(VERSION 3.14)
project(ZtaCoreLib)

add_library(ZtaCore STATIC
    ZtaCore.cpp
    ZtaPng.cpp
)

target_include_directories(ZtaCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(ZtaCore PUBLIC cxx_std_17)
