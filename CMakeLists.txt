cmake_minimum_required(VERSION 3.15...4.1)

project(ztalib 
    VERSION 0.7.0
    LANGUAGES CXX
)

add_library(ztalib SHARED 
    src/data/ZtaFrameBuffer.cpp
    src/parsers/ZtaF.cpp
    src/parsers/PalF.cpp
)

target_include_directories(ztalib 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(ztalib PUBLIC cxx_std_17)

set_target_properties(ztalib PROPERTIES 
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

include(GNUInstallDirs)

install(TARGETS ztalib
    EXPORT ztalibTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install (EXPORT ztalibTargets
    FILE ztalibTargets.cmake
    NAMESPACE ztalib::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ztalib
)