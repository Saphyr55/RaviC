#include "vm/compiler.hpp"
#include "lexer.hpp"

namespace VM {

    void Compiler::Compile(std::string_view source) {
        Lexer lexer(source);
        auto& tokens = lexer.Scan();

    }

    Compiler::Compiler(/* args */)
    {
    }

    Compiler::~Compiler()
    {
    }
    
}
