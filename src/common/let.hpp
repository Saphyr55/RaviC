#pragma once

#include <string>

namespace Obj {

struct Let {
public:
    std::string type = "any";
    std::string name = "_";
    bool is_mutable = false;
};

}

