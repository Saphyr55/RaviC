#pragma once

#include <string>

namespace VM {
    
class Compiler {

public:
    static void Compile(std::string_view source);

public:
    Compiler();
    ~Compiler();

};


}
