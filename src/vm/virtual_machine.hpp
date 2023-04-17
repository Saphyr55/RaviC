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
	Negate,
	Add,
    NotEqual,
    Equal,
    LessEqual,
    Less,
    GreaterEqual,
    Greater,
    Substract,
	Multiply,
	Divide,
	ConstantLong,
    True,
    False,
    Null,
    Not,
    Print,
    Pop,
    Store,
    Load
};

enum class InterpreteResult {
	OK = 0,
	COMPILE_ERROR,
	RUNTIME_ERROR
};

class RVM {

public:
    void RuntimeError(const char* format, ...);
	InterpreteResult Run(std::string_view source);
	InterpreteResult Run();
	Byte Read8();
	Common::Value ReadConstant();
	Common::Value ReadConstantLong();
	Chunk& CurrentChunk();
    void BinaryNumber(const std::function<double(double, double)>& op);
    void Free();
    inline Memory& GetMemory() { return m_memory; }
    void FinishInterprete();

public:
	explicit RVM(Chunk& c);
	RVM() = default;
	RVM(const RVM&) = default;
	RVM(RVM&&) = default;
	~RVM() = default;

private:
	Memory m_memory;
	std::size_t m_index_pc = 0;
	Byte m_pc = OpCode::End;
	Chunk m_chunk;
    bool m_has_runtime_error = false;
    bool m_finish_interprete = false;
};

}
