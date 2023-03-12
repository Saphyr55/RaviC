#include "vm/compiler.hpp"
#include "vm/virtual_machine.hpp"
#include "analysis/lexer.hpp"
#include "analysis/parser.hpp"

namespace VM {

    void Compiler::Compile(RVM& vm, std::string_view source) {
        auto& current_chunk = vm.CurrentChunk();
        Analysis::Lexer lexer(source);
        Analysis::Parser parser(current_chunk, lexer);

        current_chunk.Write8(OpCode::Return);
    }

    Compiler::Compiler(/* args */)
    {
    }

    Compiler::~Compiler()
    {
    }
    
}
