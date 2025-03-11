#include "nanobind/nanobind.h"
#include "nanobind/stl/string.h"

#include <string>

namespace nb = nanobind;

using namespace nb::literals;

namespace muhkuk {
    struct Pet {
        std::string name;
    };
    
    struct Dog : Pet {
        std::string bark() const { return name + ": woof!"; }
    };
}

using namespace muhkuk;

NB_MODULE(okapi, m) {
    nb::class_<Pet>(m, "Pet")
       .def(nb::init<const std::string &>())
       .def_rw("name", &Pet::name);

    nb::class_<Dog, Pet /* <- C++ parent type */>(m, "Dog")
        .def(nb::init<const std::string &>())
        .def("bark", &Dog::bark);
}
