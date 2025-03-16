#include "nanobind/nanobind.h"
#include "nanobind/stl/string.h"

#include <string>

namespace nb = nanobind;

using namespace nb::literals;

namespace okapi {
    struct Spectrum {
        virtual float evaluate(float lambda) const = 0;
    };
    
    class ConstantSpectrum : Spectrum {
    public:
        ConstantSpectrum(float value) : value(value) {}
        float evaluate(float lambda) const override { return value; }

    private:
        float value;
    };
}

using namespace okapi;

NB_MODULE(okapi, m) {
    
    nb::class_<Spectrum>(m, "Spectrum")
        .def("evaluate", &Spectrum::evaluate);
  
    nb::class_<ConstantSpectrum, Spectrum>(m, "ConstantSpectrum")
        .def(nb::init<float>())
        .def("evaluate", &ConstantSpectrum::evaluate);
}
