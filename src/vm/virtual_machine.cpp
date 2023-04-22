#include <iostream>
#include <cstring>
#include <cstdarg>
#include "vm/virtual_machine.hpp"
#include "vm/compiler.hpp"
#include "instruction.hpp"

namespace VM {

    void RVM::CompileError(const char* format, ...){
        m_has_compiler_error = true;
        std::va_list args;
        va_start(args, format);
        std::vfprintf(stderr, format, args);
        va_end(args);
        std::fputs("\n", stderr);
        std::fprintf(stderr, "[line %zu] in script\n", m_chunk.m_current_line - 1);
    }

    void RVM::RuntimeError(const char* format, ...) {
        m_has_runtime_error = true;
        std::va_list args;
        va_start(args, format);
        std::vfprintf(stderr, format, args);
        va_end(args);
        std::fputs("\n", stderr);
        std::size_t instruction = m_pc - m_chunk.m_current_line - 1;
        std::size_t line = m_chunk.m_lines[instruction];
        std::fprintf(stderr, "[line %zu] in script\n", line);
        Free();
    }

    void RVM::BinaryNumber(const std::function<double(double, double)>& op) {
        auto& memory = m_chunk.GetMemory();
        if (!CheckMemoryNumber(memory)) { return; }
        double vb      = Cast<Common::VFloat64>(memory.Pop())->Get();
        double va      = Cast<Common::VFloat64>(memory.Pop())->Get();
        double result  = op(va, vb);
        memory.Write(Common::Value::OfFloat64(result));
    }

    void RVM::LogicalOp(const std::function<bool(bool, bool)> &op) {
        auto& memory = m_chunk.GetMemory();
        if (!memory.Peek(0)->Is(Common::ValType::TBoolean) ||
            !memory.Peek(1)->Is(Common::ValType::TBoolean) ) {
            RuntimeError("Operands must be numbers.");
            return;
        }
        bool  vb   = Cast<Common::VBoolean>(memory.Pop())->Get();
        bool  va   = Cast<Common::VBoolean>(memory.Pop())->Get();
        bool result  = op(va, vb);
        memory.Write(Common::Value::OfBoolean(result));
    }

    void RVM::ComparisonNumber(const std::function<bool(double, double)>& op) {
        auto& memory = m_chunk.GetMemory();
        if (!CheckMemoryNumber(memory)) { return; }
        double  vb   = Cast<Common::VFloat64>(memory.Pop())->Get();
        double  va   = Cast<Common::VFloat64>(memory.Pop())->Get();
        bool result  = op(va, vb);
        memory.Write(Common::Value::OfBoolean(result));
    }

    bool RVM::CheckMemoryNumber(Memory& memory) {
        if (!memory.Peek(0)->IsNumber() ||
            !memory.Peek(1)->IsNumber()) {
            RuntimeError("Operands must be numbers.");
            return false;
        }
        return true;
    }

	Byte RVM::Read8() {
		
        Byte pc = m_pc;
        if (m_index_pc + 1 < m_chunk.m_bytes.size())
            m_pc = m_chunk.m_bytes[++m_index_pc];

		return pc;
	}

    Ref<Common::Value> RVM::ReadConstant() {
		return m_chunk.m_memory.Get(Read8());
	}

    Ref<Common::Value> RVM::ReadConstantLong() {
		Byte byte1 = Read8();
		Byte byte2 = Read8();
		Byte bytes[2] = { byte1, byte2 };

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
        m_chunk.Disassemble();
#endif

        std::cout << "--------------------------\n";
        std::cout << "-----  Execution...  -----\n";
        std::cout << "--------------------------\n";

        while (m_index_pc < m_chunk.m_bytes.size()) {

            if (m_has_runtime_error)
                return InterpreteResult::RUNTIME_ERROR;

            if (m_finish_interprete)
                return InterpreteResult::OK;

            auto instr_bytecode = static_cast<OpCode>(Read8());
            auto instr_func = Instruction::RVMInstructions[instr_bytecode];
            instr_func(*this, m_chunk, m_chunk.GetMemory());

        }

        return InterpreteResult::OK;
    }

    void RVM::Free() {
        m_chunk.Free();
    }

    void RVM::FinishInterprete() {
        m_finish_interprete = true;
    }

    RVM::RVM(int argc, char** argv) { }



}

