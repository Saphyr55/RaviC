#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"

int main(int argc, char** argv) {

	Lexer lexer("func main () { let mut m : i32 = 3; }");
	std::vector<Ref<Token>> tokens = lexer.Scan();

	for (auto& t : tokens) {
		if (t != nullptr) {
			std::cout << t->Text << "\n";
		}
	}
	 

	Parser p(tokens);
	p.Parse();

    return 0;
}
