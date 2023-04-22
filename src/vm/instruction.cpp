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
            { OpCode::BinaryAnd,    &Instruction::BinaryAnd         },
            { OpCode::BinaryOr,     &Instruction::BinaryOr          },
            { OpCode::LogicalAnd,   &Instruction::LogicalAnd        },
            { OpCode::LogicalOr,    &Instruction::LogicalOr         },
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
        Memory::PrintValue(memory.Pop(), true);
    }

    void Instruction::Not(RVM &rvm, Chunk&, Memory& memory) {
        auto value = memory.Pop();
        if (!value->Is(Common::ValType::TBoolean)) {
            rvm.RuntimeError("Operand must be a boolean.");
            return;
        }
        auto result = !Cast<Common::VBoolean>(value)->Get();
        memory.Write(Common::Value::OfBoolean(result));
    }

    void Instruction::Negate(RVM& rvm, Chunk&, Memory& memory) {
        auto negate_value = memory.Pop();
        if (!negate_value->IsNumber()) {
            rvm.RuntimeError("Operand must be a number.");
            return;
        }
        auto result = -Cast<Common::VFloat64>(negate_value)->Get();
        memory.Write(Common::Value::OfFloat64(result));
    }

    void Instruction::Add(RVM& rvm, Chunk&, Memory& memory) {

        bool firstIsString = memory.Peek()->IsString();
        bool secondIsString = memory.Peek(1)->IsString();

        if (firstIsString && secondIsString) {
            auto first = memory.Pop();
            auto second = memory.Pop();
            auto result =
                    Cast<Common::VString>(second)->Get() +
                    Cast<Common::VString>(first)->Get();
            memory.Write(Common::Value::OfString(result));
            return;
        }

        rvm.BinaryNumber(std::plus());
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
        memory.Write(Common::Value::OfTrue());
    }

    void Instruction::False(RVM&, Chunk&, Memory& memory) {
        memory.Write(Common::Value::OfFalse());
    }

    void Instruction::Null(RVM&, Chunk&, Memory& memory) {
        memory.Write(Common::Value::OfNull());
    }

    void Instruction::ConstantLong(RVM& rvm, Chunk&, Memory& memory) {
        auto constant = rvm.ReadConstantLong();
        memory.Write(constant);
    }

    void Instruction::NotEqual(RVM&, Chunk&, Memory& memory) {
        auto v1 = memory.Pop();
        auto v2 = memory.Pop();
        auto newValue = Common::Value::OfBoolean(!(*v1 == v2));
        memory.Write(newValue);
    }

    void Instruction::Equal(RVM&, Chunk&, Memory& memory) {
        auto v1 = memory.Pop();
        auto v2 = memory.Pop();
        auto newValue = Common::Value::OfBoolean(*v1 == v2);
        memory.Write(newValue);
    }

    void Instruction::LessEqual(RVM& rvm, Chunk&, Memory&) {
        rvm.ComparisonNumber([](double a, double b) -> bool {
            return a <= b;
        });
    }

    void Instruction::Less(RVM& rvm, Chunk&, Memory&) {
        rvm.ComparisonNumber([](double a, double b) -> bool {
            return a < b;
        });
    }

    void Instruction::GreaterEqual(RVM& rvm, Chunk&, Memory&) {
        rvm.ComparisonNumber([](double a, double b) -> bool {
            return a >= b;
        });
    }

    void Instruction::Greater(RVM& rvm, Chunk&, Memory&) {
        rvm.ComparisonNumber([](double a, double b) -> bool {
            return a > b;
        });
    }

    void Instruction::Pop(RVM&, Chunk&, Memory& memory) {
        memory.Pop();
    }

    void Instruction::Store(RVM& rvm, Chunk& chunk, Memory& memory) {
        auto name = rvm.ReadConstant();
        if (!name->IsString()) {
            rvm.RuntimeError("Must be named variable.");
            return;
        }
        auto value = memory.Pop();
        std::string key = Cast<Common::VString>(name)->Get();
        chunk.Add(key, value);
    }

    void Instruction::BinaryAnd(RVM& rvm, Chunk&, Memory& memory) {

    }

    void Instruction::BinaryOr(RVM& rvm, Chunk&, Memory&) {

    }

    void Instruction::LogicalAnd(RVM& rvm, Chunk&, Memory&) {
        rvm.LogicalOp([](bool a, bool b) -> bool {
            return a && b;
        });
    }

    void Instruction::LogicalOr(RVM& rvm, Chunk&, Memory&) {
        rvm.LogicalOp([](bool a, bool b) -> bool {
            return a && b;
        });
    }

    void Instruction::Load(RVM& rvm, Chunk& chunk, Memory& memory) {

        auto name_value = rvm.ReadConstant();
        auto name = Cast<Common::VString>(name_value)->Get();
        auto value = chunk.Get(name);

        if (!value) {
            rvm.RuntimeError("Undefined variable '%s'.", name);
            return;
        }

        memory.Write(value);
    }

}
