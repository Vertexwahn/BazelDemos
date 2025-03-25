#include "nanobind/nanobind.h"
#include "nanobind/stl/string.h"

#include <string>
#include <vector>

namespace nb = nanobind;

using namespace nb::literals;

namespace okapi {
    constexpr float LAMBDA_MIN = 360, LAMBDA_MAX = 830;

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

    class DenselySampledSpectrum : Spectrum {
    public:
        DenselySampledSpectrum(const Spectrum& spectrum) {
            for(int i = LAMBDA_MIN; i <= LAMBDA_MAX; i++) {
                values.emplace_back(spectrum.evaluate(i));
            }
        }
        float evaluate(float lambda) const override {
            return values[lambda - LAMBDA_MIN];
        }

    private:
        std::vector<float> values;
    };
}

using namespace okapi;

NB_MODULE(okapi, m) {
    nb::class_<Spectrum>(m, "Spectrum")
        .def("evaluate", &Spectrum::evaluate);
  
    nb::class_<ConstantSpectrum, Spectrum>(m, "ConstantSpectrum")
        .def(nb::init<float>())
        .def("evaluate", &ConstantSpectrum::evaluate);

    nb::class_<DenselySampledSpectrum, Spectrum>(m, "DenselySampledSpectrum")
        .def("evaluate", &DenselySampledSpectrum::evaluate);
}
