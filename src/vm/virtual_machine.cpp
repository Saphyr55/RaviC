#include <iostream>
#include <cstring>
#include <cstdarg>
#include "vm/virtual_machine.hpp"
#include "vm/compiler.hpp"
#include "instruction.hpp"

namespace VM {

	RVM::RVM(Chunk& c) : m_chunk(c), m_pc(OpCode::End), m_index_pc(0) { }

    void RVM::RuntimeError(const char* format, ...) {

        m_has_runtime_error = true;
        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
        fputs("\n", stderr);

        std::size_t instruction = m_pc - m_chunk.m_current_line - 1;
        std::size_t line = m_chunk.m_lines[instruction];
        fprintf(stderr, "[line %zu] in script\n", line);
        Free();
    }

    void RVM::BinaryNumber(const std::function<double(double, double)>& op) {
        if (!m_memory.Peek(0).IsNumber() || !m_memory.Peek(1).IsNumber() ) {
            RuntimeError("Operands must be numbers.");
            return;
        }
        auto b = m_memory.Pop().As.Number;
        auto a = m_memory.Pop().As.Number;
        m_memory.Write(Common::Value(op(a, b)));
    }

	Byte RVM::Read8() {
		
		Byte pc = m_pc;
		if (m_index_pc + 1 < m_chunk.m_bytes.size())
			m_pc = m_chunk.m_bytes[++m_index_pc];
		
		return pc;
	}

	Common::Value RVM::ReadConstant() {

		return m_chunk.m_memory.Get(Read8());
	}

	Common::Value RVM::ReadConstantLong() {

		Byte byte1 = Read8();
		Byte byte2 = Read8();
		Byte bytes[] = { byte1, byte2 };

		std::size_t addr;
		std::memcpy(&addr, bytes, sizeof(std::size_t));
		return m_chunk.m_memory.GetHandle()[addr];
	}

	Chunk& RVM::CurrentChunk() {
		
		return m_chunk;
	}

	InterpreteResult RVM::Run(std::string_view source) {
		
		Compiler compiler(*this, source);
		compiler.Compile();
        m_pc = m_chunk.m_bytes.front();
		return Run();
	}

	InterpreteResult RVM::Run() {

#ifdef DEBUG_TRACE_EXECUTION
        m_chunk.Disassemble("");
#endif

		while (m_index_pc < m_chunk.m_bytes.size()) {

            if (m_has_runtime_error)
                return InterpreteResult::RUNTIME_ERROR;

            if (m_finish_interprete)
                return InterpreteResult::OK;

            auto instr_bytecode = static_cast<OpCode>(Read8());
            auto instr_func = Instruction::RVMInstructions[instr_bytecode];
            instr_func(*this, m_chunk, m_memory);

        }

        return InterpreteResult::OK;
    }

    void RVM::Free() {
        m_memory.Free();
        m_chunk.Free();
    }

    void RVM::FinishInterprete() {
        m_finish_interprete = true;
    }


}

