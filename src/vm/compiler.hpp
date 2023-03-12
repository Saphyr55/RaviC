#pragma once

#include <string>

namespace VM {
   
class RVM;

class Compiler {

public:
    static void Compile(RVM& vm, std::string_view source);

public:
    Compiler();
    ~Compiler();

};


}
