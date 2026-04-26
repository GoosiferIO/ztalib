include(FetchContent)

FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG v3.0.4
)

FetchContent_MakeAvailable(pybind11)

pybind11_add_module(pyzta 
    src/pyzta.cpp
)

target_link_libraries(pyzta PRIVATE ztalib)
target_include_directories(pyzta PRIVATE ${CMAKE_SOURCE_DIR}/include)