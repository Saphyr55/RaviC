#pragma once

#include <string>

#include "analysis/lexer.hpp"
#include "analysis/parser.hpp"

namespace VM {
   
class RVM;

class Compiler {

public:
    void Compile();

public:
    Compiler(RVM& vm, std::string_view source);
    Compiler(const Compiler&) = default;
    Compiler(Compiler&&) = default;
    ~Compiler() = default;
   
private:
    RVM& vm;
    Analysis::Lexer lexer;
    Analysis::Parser parser;

};


}
