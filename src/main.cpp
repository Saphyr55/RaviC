#include <iostream>
#include "lexer.hpp"

int main(int argc, char** argv) {

	Lexer lexer("func main () { let mut m : i32 = 3; }");
	
	std::vector<RToken> tokens = lexer.Scan();
	
	for (auto& t : tokens) {
		std::cout << t->Text << "\n";
	}
	
    return 0;
}
