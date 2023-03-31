#include "vm/compiler.hpp"
#include "vm/virtual_machine.hpp"

namespace VM {

    void Compiler::Compile() {
        lexer.Scan();
        parser.Expression();
        vm.CurrentChunk().Write8(OpCode::End);
    }

    Compiler::Compiler(RVM& vm, std::string_view source)
        : lexer(source), parser(vm.CurrentChunk(), lexer), vm(vm)
    {
       
    }

   
}
