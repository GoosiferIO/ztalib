cmake_minimum_required(VERSION 3.15...4.1)

project(ztalib 
    VERSION 0.7.0
    LANGUAGES CXX
)

# ------------- Sources -------------

add_library(ztalib SHARED 
    src/data/ZtaFrameBuffer.cpp
    src/parsers/ZtaF.cpp
    src/parsers/PalF.cpp
)

#------------ Include and def public vs private headers ------------

target_include_directories(ztalib 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(ztalib PUBLIC cxx_std_17)

# ------------- Do not mangle --------------

set_target_properties(ztalib PROPERTIES 
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

# ------------- Packaging -------------

include(GNUInstallDirs)

# ------------- Installation -------------

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

# ------------- Options -------------

option(BUILD_PY_BINDINGS "Build Python bindings" OFF)

if(BUILD_PY_BINDINGS)
    add_subdirectory(bindings)
    find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
    install(TARGETS pyzta
        DESTINATION ${Python3_SITELIB}
    )
endif()