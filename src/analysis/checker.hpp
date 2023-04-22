#pragma once

#include "vm/virtual_machine.hpp"
#include "analysis/parser.hpp"

namespace Analysis {

class Checker {

public:
    void LetDeclaration(Obj::Let& let);
    bool TypeExist(Ref<Token> tk);

public:
    Checker(VM::RVM& rvm);
    Checker(const Checker&) = default;
    Checker(Checker&&)  = default;
    ~Checker() = default;

private:
    VM::RVM& m_rvm;
    VM::Chunk& m_current_chunk;

};

}
