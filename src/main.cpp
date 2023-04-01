#include <iostream>
#include "analysis/lexer.hpp"
#include "vm/chunk.hpp"
#include "vm/memory.hpp"
#include "vm/virtual_machine.hpp"

std::string source = "func main () { let mut m : i32 = 3 if (1 != 0) return 0; }";

int main(int argc, char** argv) {
	
	/*
	Analysis::Lexer lexer(source);
	
	for (int i = 0; i < 24; i++) {

		auto t = lexer.PeekNextToken();
		std::cout << t->Text << " " << Analysis::Token::ToString(t->KindType) << "\n";
	
	}
	*/
	
	/*
	VM::Chunk chunk;
	
	chunk.WriteConstant(50); // - 50
	chunk.Write8(VM::OpCode::Negate);
	chunk.WriteConstant(20); // 20
	chunk.Write8(VM::OpCode::Substract); // - 50 - 20
	chunk.WriteConstant(20);
	chunk.Write8(VM::OpCode::Add);
	chunk.Write8(VM::OpCode::End);
	
	VM::RVM vm(chunk);
	vm.Run();
	*/
	
	VM::RVM rvm;
	rvm.Run("2 + (6 * 2) ");
	
    return 0;
}
