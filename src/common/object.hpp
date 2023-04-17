#pragma once

#include <cstring>
#include <string_view>
#include "common/common.hpp"

namespace Common {

class StringObject;

class Object {

public:
    enum class Type {
        String,
    };

public:
    explicit Object(Type type);

public:
    Type OType;
};

using ObjectPtr = Object*;

class StringObject : public Object {

public:
    explicit StringObject(std::string_view value);

public:
    std::string String;
};

using StringObjectPtr = StringObject*;

}

