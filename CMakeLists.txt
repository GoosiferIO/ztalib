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

# ------------- Options -------------

option(BUILD_PY_BINDINGS "Build Python bindings" OFF)
option(BUILD_CPP_LIB "Build C++ library, headers, and CMake targets" ON)

if(BUILD_PY_BINDINGS) # python bindings only
    find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
    add_subdirectory(bindings)
    install(TARGETS pyzta
        RUNTIME DESTINATION ${Python3_SITELIB}
        LIBRARY DESTINATION ${Python3_SITELIB}
    )
    install(TARGETS ztalib
        RUNTIME DESTINATION ${Python3_SITELIB}
        LIBRARY DESTINATION ${Python3_SITELIB}
    )
endif() 

if(BUILD_CPP_LIB) # c++ library only
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
endif()