#pragma once

#include <stack>
#include "vm/chunk.hpp"
#include "common/common.hpp"

#define DEBUG_TRACE_EXECUTION

namespace VM {

enum OpCode : std::size_t {
	Constant,
	Return,
	Negate,
	Add,
	Substract,
	Multiply,
	Divide,
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
	Chunk& CurrentChunk();

public:
	RVM(Chunk& c);

private:
	void BinaryAdd();
	void BinaryMul();
	void BinarySub();
	void BinaryDiv();

private:
	std::stack<Value> m_values;
	std::size_t m_index_pc;
	Byte m_pc;
	Chunk m_chunk;
};

}
