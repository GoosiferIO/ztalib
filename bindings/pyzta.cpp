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
        .def("has_background", &ZtaData::hasBackground);

    py::class_<ZtaFrame>(m, "ZtaFrame")
        .def("frame_size", &ZtaFrame::frameSize)
        .def("height", &ZtaFrame::height)
        .def("width", &ZtaFrame::width)
        .def("y_offset", &ZtaFrame::y)
        .def("x_offset", &ZtaFrame::x)
        .def_readonly("pixel_sets", &ZtaFrame::pixelSets);

    py::class_<ZtaInfo>(m, "ZtaInfo")
        .def("speed", &ZtaInfo::speed)
        .def("frame_count", &ZtaInfo::frameCount);

    py::class_<ZtaFrameBufferObject>(m, "ZtaFrameBufferObject")
        .def_readonly("pixels", &ZtaFrameBufferObject::pixels)
        .def("width", &ZtaFrameBufferObject::width)
        .def("height", &ZtaFrameBufferObject::height)
        .def("offset_x", &ZtaFrameBufferObject::offsetX)
        .def("offset_y", &ZtaFrameBufferObject::offsetY)
        .def("channels", &ZtaFrameBufferObject::channels);

    py::class_<ZtaFrameBuffer>(m, "ZtaFrameBuffer")
        .def_readonly("frames", &ZtaFrameBuffer::frames);

    py::class_<PalF, std::shared_ptr<PalF>>(m, "PalF")
        .def(py::init<>())
        .def("location", &PalF::location)
        .def("set_location", &PalF::location)
        .def("location_size", &PalF::locationSize)
        .def("set_location_size", &PalF::locationSize)
        .def("color_model", &PalF::colorModel)
        .def("set_color_model", &PalF::colorModel)
        .def("num_colors", &PalF::numColors)
        .def("set_num_colors", &PalF::numColors)
        .def("get_color", &PalF::getColor)
        .def("load", &PalF::load);

    py::class_<ZtaF>(m, "ZtaF")
        .def(py::init<>())
        .def("load", &ZtaF::load, 
            py::arg("file_name"), 
            py::arg("color_profile") = 0, 
            py::arg("io_pal") = "")
        .def("data", &ZtaF::data)
        .def("get_frame_buffer", &ZtaF::getFrameBuffer);
} 