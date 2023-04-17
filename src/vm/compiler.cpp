#include "vm/compiler.hpp"
#include "vm/virtual_machine.hpp"

namespace VM {

    void Compiler::Compile() {

        parser.Advance();

        while (!parser.IsAtEnd()) {
            parser.Decleration();
        }

        parser.Consume(Analysis::Token::Kind::TkEOF, "Wait end expression.");
		vm.CurrentChunk().Write8(OpCode::End);

    }

    Compiler::Compiler(RVM& vm, std::string_view source)
        : lexer(source), parser(vm.CurrentChunk(), lexer), vm(vm)
    {

    }

   
}
