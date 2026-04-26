cmake_minimum_required(VERSION 3.15...4.1)

project(ztalib 
    VERSION 0.8.0
    LANGUAGES CXX
)

# ------------- Sources -------------

if(BUILD_PY_BINDINGS)
    add_library(ztalib STATIC # static for python bindings
        src/data/ZtaFrameBuffer.cpp
        src/parsers/ZtaF.cpp
        src/parsers/PalF.cpp
    )
    set_target_properties(ztalib PROPERTIES POSITION_INDEPENDENT_CODE ON)
else() # c++ can be shared
    add_library(ztalib SHARED
        src/data/ZtaFrameBuffer.cpp
        src/parsers/ZtaF.cpp
        src/parsers/PalF.cpp
    )
    set_target_properties(ztalib PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# ------------- Export config (version) -------------

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ztalib/data/ZtaVersion.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/ztalib/data/ZtaVersion.h
)

#------------ Include and def public vs private headers ------------

target_include_directories(ztalib 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(ztalib PUBLIC cxx_std_17)
target_compile_options(ztalib PRIVATE 
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

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
    find_package(Python3 COMPONENTS Interpreter Development.Module REQUIRED)
    add_subdirectory(bindings)

    if(DEFINED SKBUILD_PLATLIB_DIR)
        set(PY_INSTALL_DIR ${SKBUILD_PLATLIB_DIR})
    else()
        set(PY_INSTALL_DIR ${Python3_SITELIB})
    endif()

    install(TARGETS pyzta
        RUNTIME DESTINATION ${PY_INSTALL_DIR}
        LIBRARY DESTINATION ${PY_INSTALL_DIR}
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