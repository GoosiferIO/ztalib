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

    py::class_<ZtaInfo>(m, "ZtaInfo")
        .def_readwrite("speed", &ZtaInfo::speed)
        .def_readwrite("frame_count", &ZtaInfo::frameCount);

    py::class_<ZtaData, std::shared_ptr<ZtaData>>(m, "ZtaData")
        .def_readonly("info", &ZtaData::info)
        .def_readonly("frames", &ZtaData::frames)
        .def_readonly("palette", &ZtaData::palette)
        .def_readwrite("has_background", &ZtaData::hasBackground);

    py::class_<ZtaFrame>(m, "ZtaFrame")
        .def_readwrite("frame_size", &ZtaFrame::frameSize)
        .def_readwrite("height", &ZtaFrame::height)
        .def_readwrite("width", &ZtaFrame::width)
        .def_readwrite("y_offset", &ZtaFrame::y)
        .def_readwrite("x_offset", &ZtaFrame::x)
        .def_readonly("pixel_sets", &ZtaFrame::pixelSets);

    py::class_<ZtaFrameBufferObject>(m, "ZtaFrameBufferObject")
        .def_property_readonly("pixels", [](const ZtaFrameBufferObject& obj) {
            return py::bytes(
                reinterpret_cast<const char*>(obj.pixels.data()), 
                obj.pixels.size()
            );
        })
        .def_readonly("width", &ZtaFrameBufferObject::width)
        .def_readonly("height", &ZtaFrameBufferObject::height)
        .def_readonly("offset_x", &ZtaFrameBufferObject::offsetX)
        .def_readonly("offset_y", &ZtaFrameBufferObject::offsetY)
        .def_readonly("channels", &ZtaFrameBufferObject::channels);

    py::class_<ZtaFrameBuffer>(m, "ZtaFrameBuffer")
        .def(py::init<const ZtaData&>())
        .def("get_buffer", &ZtaFrameBuffer::getBuffer);

    py::class_<PalF::Color>(m, "PalFColor")
        .def_readonly("r", &PalF::Color::r)
        .def_readonly("g", &PalF::Color::g)
        .def_readonly("b", &PalF::Color::b)
        .def_readonly("a", &PalF::Color::a);

    py::class_<PalF, std::shared_ptr<PalF>>(m, "PalF")
        .def(py::init<>())
        .def("location", py::overload_cast<std::string>(&PalF::location))
        .def("location", py::overload_cast<>(&PalF::location))
        .def("location_size", py::overload_cast<uint32_t>(&PalF::locationSize))
        .def("location_size", py::overload_cast<>(&PalF::locationSize))
        .def("color_model", py::overload_cast<int>(&PalF::colorModel))
        .def("color_model", py::overload_cast<>(&PalF::colorModel))
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