#include <iostream>
#include "lexer.hpp"
#include "vm/chunk.hpp"
#include "vm/memory.hpp"
#include "vm/virtual_machine.hpp"

std::string source = "func main () { let mut m : i32 = 3 if (1 != 0) return 0; }";

int main(int argc, char** argv) {
	/*
	Lexer lexer();
	std::vector<Ref<Token>> tokens = lexer.Scan();

	for (auto& t : tokens) {
		std::cout << t->Text << " " << Token::ToString(t->KindType) << "\n";
	}
	 
	Parser p(tokens);
	p.Parse();*/

	VM::Chunk chunk;

	chunk.Write8(VM::OpCode::Constant); // -50
	auto value = chunk.AddConstant(50);
	chunk.Write8(value);
	chunk.Write8(VM::OpCode::Negate); 

	chunk.WriteConstant(20); // 20

	chunk.Write8(VM::OpCode::Substract); // - 50 - 20 

	chunk.Write8(VM::OpCode::Return);
	
	VM::RVM rvm(chunk);
	rvm.Run(source);
	
    return 0;
}
