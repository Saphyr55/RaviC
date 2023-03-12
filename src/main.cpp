#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"
#include "vm/chunk.hpp"
#include "vm/memory.hpp"

int main(int argc, char** argv) {
	/*
	Lexer lexer("func main () { let mut m : i32 = 3 if (1 != 0) return 0; }");
	std::vector<Ref<Token>> tokens = lexer.Scan();

	for (auto& t : tokens) {
		std::cout << t->Text << " " << Token::ToString(t->KindType) << "\n";
	}
	 
	Parser p(tokens);
	p.Parse();*/

	VM::Chunk chunk;

	chunk.Write(VM::OpCode::Return);

	chunk.Write(VM::OpCode::Constant);
	chunk.Write(chunk.AddConstant(20));

	chunk.Disassemble("--- Test Chunk ---");

    return 0;
}
