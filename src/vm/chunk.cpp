#include <iostream>
#include <cstdio>
#include "vm/chunk.hpp"
#include "vm/memory.hpp"

namespace VM {

	void Chunk::Disassemble(std::string_view name) {
		std::cout << name << "\n";
		for (std::size_t offset = 0; offset < m_bytes.size();) {
			offset = Disassemble(offset);
		}
	}

	std::size_t Chunk::Disassemble(std::size_t offset) {

		std::printf("%04zu ", offset);

		Byte instruction = m_bytes[offset];

		switch (instruction) {

		case OpCode::Return :
			return SimpleInstruction("Return", offset);
		case OpCode::Constant:
			return ConstantInstruction("Constant", offset);
		default:
			printf("Unknown opcode %d\n", instruction);
			return offset + 1;
		}
	}

	std::size_t Chunk::SimpleInstruction(std::string_view name, std::size_t offset) {
		std::cout << name << "\n";
		return offset + 1;
	}

	std::size_t Chunk::ConstantInstruction(std::string_view name, std::size_t offset) {
		Byte constant = m_bytes[offset + 1];
		std::printf("%-16s %4d '", name.data(), constant);
		Memory::PrintValue(m_memory.GetHandle()[constant]);
		std::printf("'\n");
		return offset + 2;
	}

	void Chunk::Write(const Byte& byte) {
		m_bytes.push_back(byte);
		m_lines.push_back(m_current_line);
	}

	std::size_t Chunk::AddConstant(const Value& value) {
		m_memory.Write(value);
		return m_memory.Size() - 1;
	}

}
