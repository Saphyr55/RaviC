#pragma once

#include <stack>
#include <functional>
#include "vm/chunk.hpp"
#include "common/common.hpp"

#define DEBUG_TRACE_EXECUTION

namespace VM {

enum OpCode : Byte {
	Constant = 0,
	End = 1,
	Negate = 2,
	Add = 3,
    NotEqual = 8,
    Equal = 7,
    LessEqual = 10,
    Less = 9,
    GreaterEqual = 12,
    Greater = 11,
    Substract = 4,
	Multiply = 5,
	Divide = 6,
	ConstantLong = 21,
    True = 13,
    False = 14,
    Null = 15,
    Not = 16,
    Print = 17,
    Pop = 18,
    Store = 19,
    Load = 20,
    BinaryAnd,
    BinaryOr,
    LogicalAnd,
    LogicalOr
};

enum class InterpreteResult {
	OK = 0,
	COMPILE_ERROR,
	RUNTIME_ERROR
};

class RVM {

public:
	void CompileError(const char* format, ...);
	void RuntimeError(const char* format, ...);
	InterpreteResult Run(std::string_view source);
	InterpreteResult Run();
	Byte Read8();
    Ref<Common::Value> ReadConstant();
    Ref<Common::Value> ReadConstantLong();
	Chunk& CurrentChunk();
    void BinaryNumber(const std::function<double(double, double)>& op);
    void ComparisonNumber(const std::function<bool(double, double)> &op);
    void LogicalOp(const std::function<bool(bool, bool)> &op);
    void Free();
    void FinishInterprete();
    bool CheckMemoryNumber(Memory &memory);

public:
	RVM() = default;
    RVM(int argc, char** argv);
	RVM(const RVM&) = default;
	RVM(RVM&&) = default;
	~RVM() = default;

private:
	std::size_t m_index_pc = 0;
	Byte m_pc = OpCode::End;
	Chunk m_chunk;
    bool m_has_runtime_error = false;
    bool m_finish_interprete = false;
	bool m_has_compiler_error = false;
};

}
