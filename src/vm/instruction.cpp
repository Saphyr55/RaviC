#include <iostream>
#include "vm/instruction.hpp"

namespace VM {

    std::map<OpCode, const std::function<void(RVM&, Chunk&, Memory&)>> Instruction::RVMInstructions = {
            { OpCode::End,          &Instruction::End               },
            { OpCode::Not,          &Instruction::Not               },
            { OpCode::Negate,       &Instruction::Negate            },
            { OpCode::Add,          &Instruction::Add               },
            { OpCode::Substract,    &Instruction::Substract         },
            { OpCode::Divide,       &Instruction::Divide            },
            { OpCode::Multiply,     &Instruction::Multiply          },
            { OpCode::Constant,     &Instruction::Constant          },
            { OpCode::ConstantLong, &Instruction::ConstantLong      },
            { OpCode::True,         &Instruction::True              },
            { OpCode::False,        &Instruction::False             },
            { OpCode::Null,         &Instruction::Null              },
            { OpCode::Greater,      &Instruction::Greater           },
            { OpCode::GreaterEqual, &Instruction::GreaterEqual      },
            { OpCode::Less,         &Instruction::Less              },
            { OpCode::LessEqual,    &Instruction::LessEqual         },
            { OpCode::Equal,        &Instruction::Equal             },
            { OpCode::NotEqual,     &Instruction::NotEqual          },
            { OpCode::Print,        &Instruction::Print             },
            { OpCode::Pop,          &Instruction::Pop               },
            { OpCode::Store,        &Instruction::Store             },
            { OpCode::Load,         &Instruction::Load              }
    };

    void Instruction::End(RVM& rvm, Chunk&, Memory&) {
        rvm.FinishInterprete();
    }

    void Instruction::Print(RVM&, Chunk&, Memory& memory) {
        auto value = memory.Pop();
        Memory::PrintValue(value, true);
    }

    void Instruction::Not(RVM &rvm, Chunk&, Memory& memory) {
        auto value = memory.Pop();
        if (!value.IsBool()) {
            rvm.RuntimeError("Operand must be a boolean.");
            return;
        }
        memory.Write(Common::Value(!value.As.Boolean));
    }

    void Instruction::Negate(RVM& rvm, Chunk&, Memory& memory) {
        auto negate_value = memory.Pop();
        if (!negate_value.IsNumber()) {
            rvm.RuntimeError("Operand must be a number.");
            return;
        }
        memory.Write(Common::Value(- negate_value.As.Number ));
    }

    void Instruction::Add(RVM& rvm, Chunk&, Memory& memory) {

        auto firstIsString = memory.Peek(0).IsString();
        auto secondIsString = memory.Peek(1).IsString();

        if (firstIsString && secondIsString) {

            auto first = memory.Pop().AsString()->String;
            auto second = memory.Pop().AsString()->String;
            memory.Write(Common::Value(std::string(second + first)));
        } else {
            rvm.BinaryNumber(std::plus());
        }
    }

    void Instruction::Substract(RVM& rvm, Chunk&, Memory&) {
        rvm.BinaryNumber(std::minus());
    }

    void Instruction::Divide(RVM& rvm, Chunk&, Memory&) {
        rvm.BinaryNumber([](double a, double b) -> double {
            return a / b;
        });
    }

    void Instruction::Multiply(RVM& rvm, Chunk&, Memory&) {
        rvm.BinaryNumber(std::multiplies());
    }

    void Instruction::Constant(RVM& rvm, Chunk&, Memory& memory) {
        auto constant = rvm.ReadConstant();
        memory.Write(constant);
    }

    void Instruction::True(RVM&, Chunk&, Memory& memory) {
        memory.Write(Common::Value(true));
    }

    void Instruction::False(RVM&, Chunk&, Memory& memory) {
        memory.Write(Common::Value(false));
    }

    void Instruction::Null(RVM&, Chunk&, Memory& memory) {
        memory.Write(Common::Value());
    }

    void Instruction::ConstantLong(RVM& rvm, Chunk&, Memory& memory) {
        auto constant = rvm.ReadConstantLong();
        memory.Write(constant);
    }

    void Instruction::NotEqual(RVM&, Chunk&, Memory& memory) {
        auto v1 = memory.Pop();
        auto v2 = memory.Pop();
        memory.Write(Common::Value(!v1.IsEqual(v2)));
    }

    void Instruction::Equal(RVM&, Chunk&, Memory& memory) {
        auto v1 = memory.Pop();
        auto v2 = memory.Pop();
        memory.Write(Common::Value(v1.IsEqual(v2)));
    }

    void Instruction::LessEqual(RVM& rvm, Chunk&, Memory& memory) {
        auto v1 = memory.Pop();
        auto v2 = memory.Pop();
        if (!v1.IsNumber() || !v2.IsNumber()) {
            rvm.RuntimeError("Operand must be a number.");
            return;
        }
        memory.Write(Common::Value(v1.AsNumber() <= v2.AsNumber()));
    }

    void Instruction::Less(RVM& rvm, Chunk&, Memory& memory) {
        auto v1 = memory.Pop();
        auto v2 = memory.Pop();
        if (!v1.IsNumber() || !v2.IsNumber()) {
            rvm.RuntimeError("Operand must be a number.");
            return;
        }
        memory.Write(Common::Value(v1.As.Number < v2.As.Number));
    }

    void Instruction::GreaterEqual(RVM& rvm, Chunk&, Memory& memory) {
        auto v1 = memory.Pop();
        auto v2 = memory.Pop();
        if (!v1.IsNumber() || !v2.IsNumber()) {
            rvm.RuntimeError("Operand must be a number.");
            return;
        }
        memory.Write(Common::Value(v1.As.Number >= v2.As.Number));
    }

    void Instruction::Greater(RVM& rvm, Chunk&, Memory& memory) {
        auto v1 = memory.Pop();
        auto v2 = memory.Pop();
        if (!v1.IsNumber() || !v2.IsNumber()) {
            rvm.RuntimeError("Operand must be a number.");
            return;
        }
        memory.Write(Common::Value(v1.As.Number > v2.As.Number));
    }

    void Instruction::Pop(RVM&, Chunk&, Memory &memory) {
        memory.Pop();
    }

    void Instruction::Store(RVM& rvm, Chunk& chunk, Memory &memory) {

        if (!memory.Peek(0).IsString()) {
            rvm.RuntimeError("Operand must be named.");
            return;
        }

        auto value = rvm.ReadConstant();
        auto name = value.AsString()->String;
        chunk.Add(name, std::make_shared<Common::Value>(memory.Pop()));
    }

    void Instruction::Load(VM::RVM& rvm, VM::Chunk& chunk, VM::Memory& memory) {
        auto name = rvm.ReadConstant().AsString()->String;
        Ref<Common::Value> value = chunk.Get(name);
        if (!value) {
            rvm.RuntimeError("Undefined variable '%s'.", name);
            return;
        }
        memory.Write(*value);
    }

}
