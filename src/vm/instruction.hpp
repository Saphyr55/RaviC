#pragma once

#include <map>

#include "vm/virtual_machine.hpp"

namespace VM {

class Instruction {

public:
    static std::map<OpCode, const std::function<void(RVM&, Chunk&, Memory&)>> RVMInstructions;

    static void Not(RVM&, Chunk&, Memory&);
    static void Negate(RVM&, Chunk&, Memory&);
    static void Constant(RVM&, Chunk&, Memory&);
    static void False(RVM&, Chunk&, Memory&);
    static void True(RVM&, Chunk&, Memory&);
    static void Add(RVM&, Chunk&, Memory&);
    static void Substract(RVM&, Chunk&, Memory&);
    static void Divide(RVM&, Chunk&, Memory&);
    static void Multiply(RVM&, Chunk&, Memory&);
    static void Null(RVM&, Chunk&, Memory&);
    static void End(RVM&, Chunk&, Memory&);
    static void ConstantLong(RVM&, Chunk&, Memory&);
    static void NotEqual(RVM&, Chunk&, Memory&);
    static void Equal(RVM&, Chunk&, Memory&);
    static void LessEqual(RVM& , Chunk&, Memory&);
    static void Less(RVM&, Chunk&, Memory&);
    static void GreaterEqual(RVM&, Chunk&, Memory&);
    static void Greater(RVM&, Chunk&, Memory& );
    static void Print(RVM&, Chunk&, Memory&);
    static void Pop(RVM&, Chunk&, Memory&);
    static void Store(RVM&, Chunk&, Memory&);
    static void Load(RVM&, Chunk &, Memory&);

};

}



