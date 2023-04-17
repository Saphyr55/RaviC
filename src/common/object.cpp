#include "common/object.hpp"

namespace Common {

    Object::Object(Type type) : OType(type) {
    }

    StringObject::StringObject(std::string_view value)
        : Object(Object::Type::String), String(value) {
    }

}


