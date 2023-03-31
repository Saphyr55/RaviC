#pragma once

#include <stack>
#include "vm/chunk.hpp"
#include "common/common.hpp"

#define DEBUG_TRACE_EXECUTION

namespace VM {

enum OpCode : Byte {
	Constant = 0,
	End = 1,
	Negate,
	Add,
	Substract,
	Multiply,
	Divide,
	Constant_Long,
};

enum class InterpreteResult {
	OK = 0,
	COMPILE_ERROR,
	RUNTIME_ERROR
};

class RVM {

public:
	InterpreteResult Run(std::string_view source);
	InterpreteResult Run();
	Byte Read8();
	Value ReadConstant();
	Value ReadConstantLong();
	Chunk& CurrentChunk();

public:
	RVM(Chunk& c);
	RVM() = default;
	RVM(const RVM&) = default;
	RVM(RVM&&) = default;
	~RVM() = default;

private:
	void BinaryAdd();
	void BinaryMul();
	void BinarySub();
	void BinaryDiv();

private:
	std::stack<Value> m_values;
	std::size_t m_index_pc = 0;
	Byte m_pc = 0;
	Chunk m_chunk;
};

}
