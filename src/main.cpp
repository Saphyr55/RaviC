#include "vm/virtual_machine.hpp"

int main(int argc, char** argv) {

    std::string source =
            "let x = 5\n"
            "x = 2\n"
            "->(x)"
            "->(6 + 5)";

	VM::RVM rvm(argc, argv);
	rvm.Run(source);
	
    return 0;
}
