#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "pybind11/stl/filesystem.h"

#include "ztalib/ZtaF.h"
#include "ztalib/data/ZtaData.h"
#include "ztalib/data/ZtaFrame.h"
#include "ztalib/data/ZtaInfo.h"
#include "ztalib/data/ZtaFrameBuffer.h"
#include "ztalib/data/ZtaFrameBufferObject.h"
#include "ztalib/PalF.h"

namespace py = pybind11;

PYBIND11_MODULE(pyzta, m) {
    m.doc() = "ZT1 animation graphics parser for Python";

    py::class_<ZtaData, std::shared_ptr<ZtaData>>(m, "ZtaData")
        .def_readonly("info", &ZtaData::info)
        .def_readonly("frames", &ZtaData::frames)
        .def_readonly("palette", &ZtaData::palette)
        .def_readonly("has_background", &ZtaData::hasBackground);

    py::class_<ZtaF>(m, "ZtaF")
        .def(py::init<>())
        .def("load", &ZtaF::load, py::arg("file_name"), py::arg("color_profile") = 0, py::arg("io_pal") = "")
        .def("data", &ZtaF::data)
        .def("get_frame_buffer", &ZtaF::getFrameBuffer);
} 