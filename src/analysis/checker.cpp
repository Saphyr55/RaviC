#include "analysis/checker.hpp"

namespace Analysis {

    Checker::Checker(VM::RVM& rvm) : m_rvm(rvm), m_current_chunk(m_rvm.CurrentChunk()) { }

    void Checker::LetDeclaration(Obj::Let& let) {
        auto type_expression = m_current_chunk.GetMemory().Peek(0);
        if (let.type == "any") {
            let.type = type_expression->ToString();
        }
        if (let.type != type_expression->ToString()) {
            throw std::exception();
        }
    }

    bool Checker::TypeExist(Ref<Token> tk){
        return true;
    }


}