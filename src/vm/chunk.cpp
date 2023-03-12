#include <iostream>
#include <cstdio>
#include "vm/chunk.hpp"
#include "vm/memory.hpp"
#include "vm/virtual_machine.hpp"

namespace VM {

	void Chunk::Disassemble(std::string_view name) {
		std::cout << name << "\n";
		for (std::size_t offset = 0; offset < m_bytes.size();) {
			offset = Disassemble(offset);
		}
	}

	std::size_t Chunk::Disassemble(std::size_t offset) {

		std::printf("%04zu ", offset);

		if (offset > 0 && m_lines[offset] == m_lines[offset - 1])
			std::cout << "   | ";
		else 
			std::printf("%4d ", m_lines[offset]);

		Byte instruction = m_bytes[offset];

		switch (instruction) {

		case OpCode::Return :
			return SimpleInstruction("Return", offset);
		case OpCode::Constant:
			return ConstantInstruction("Constant", offset);
		case OpCode::Negate:
			return SimpleInstruction("Negate", offset);
		case OpCode::Add:
			return  SimpleInstruction("Add", offset);
		case OpCode::Substract:
			return  SimpleInstruction("Substract", offset);
		case OpCode::Multiply:
			return  SimpleInstruction("Multiply", offset);
		case OpCode::Divide:
			return  SimpleInstruction("Divide", offset);
		default:
			std::cout << "Unknown opcode" << instruction << "\n";
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

	void Chunk::Write8(const Byte& byte) {
		m_bytes.push_back(byte);
		m_lines.push_back(m_current_line);
	}

	void Chunk::Write16(const Byte& byte1, const Byte& byte2) {
		Write8(byte1);
		Write8(byte2);
	}

	void Chunk::WriteConstant(const Value& value) {
		Write16(OpCode::Constant, AddConstant(value));
	}

	std::size_t Chunk::AddConstant(const Value& value) {
		m_memory.Write(value);
		return m_memory.Size() - 1;
	}

}
