//
// Copyright ${year} ${copyright_holder}
//
// ${license}
//

#pragma once

#include <uhd/rfnoc/block_controller_factory_python.hpp>
#include <rfnoc/${MODULE_NAME}/${blockname}_block_control.hpp>

using namespace rfnoc::gain;

void export_${blockname}_block_control(py::module& m)
{
    py::class_<${blockname}_block_control, ${blockname}_block_control::sptr>(m, "${blockname}_block_control")
        .def(py::init(
            &uhd::rfnoc::block_controller_factory<${blockname}_block_control>::make_from))

        ;
}