#include <iostream>
#include <cstdio>
#include <bitset>
#include <cmath>
#include <utility>
#include "vm/chunk.hpp"
#include "vm/memory.hpp"
#include "vm/virtual_machine.hpp"

namespace VM {

	void Chunk::Disassemble() {

        std::cout << "---------------------------" << "\n";
        std::cout << "- Disassemble bytecode... -" << "\n";
        std::cout << "---------------------------" << "\n";

        for (std::size_t offset = 0; offset < m_bytes.size();) {
            offset = Disassemble(offset);
		}
	}

	std::size_t Chunk::Disassemble(std::size_t offset) {

		std::printf("%08zX ", offset);

		if (offset > 0 && m_lines[offset] == m_lines[offset - 1])
			std::cout << "   | ";
		else 
			std::printf("%4d ", m_lines[offset]);

		Byte instruction = m_bytes[offset];

		switch (instruction) {
            case OpCode::Not:               return SimpleInstruction("Not",          offset);
            case OpCode::End :              return SimpleInstruction("End",          offset);
            case OpCode::Negate:            return SimpleInstruction("Negate",       offset);
            case OpCode::Add:               return SimpleInstruction("Add",          offset);
            case OpCode::Substract:         return SimpleInstruction("Substract",    offset);
            case OpCode::Multiply:          return SimpleInstruction("Multiply",     offset);
            case OpCode::Divide:            return SimpleInstruction("Divide",       offset);
            case OpCode::Null:              return SimpleInstruction("Null",         offset);
            case OpCode::False:             return SimpleInstruction("False",        offset);
            case OpCode::True:              return SimpleInstruction("True",         offset);
            case OpCode::Less:              return SimpleInstruction("Less",         offset);
            case OpCode::LessEqual:         return SimpleInstruction("LessEqual",    offset);
            case OpCode::Greater:           return SimpleInstruction("Greater",      offset);
            case OpCode::GreaterEqual:      return SimpleInstruction("GreaterEqual", offset);
            case OpCode::Equal:             return SimpleInstruction("Equal",        offset);
            case OpCode::NotEqual:          return SimpleInstruction("NotEqual",     offset);
            case OpCode::Print:             return SimpleInstruction("Print",        offset);
            case OpCode::Pop:               return SimpleInstruction("Pop",          offset);

            case OpCode::Store:             return ConstantInstruction("Store",             offset);
            case OpCode::Load:              return ConstantInstruction("Load",              offset);
            case OpCode::Constant:          return ConstantInstruction("Constant",          offset);
            case OpCode::ConstantLong:      return ConstantInstructionLong("Constant Long", offset);

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

		Byte addr = m_bytes[offset + 1];
		std::printf("%-16s %4d '", name.data(), addr);
		Memory::PrintValue(m_memory.Get(addr), false, true);
		std::printf("'\n");
		return offset + 2;
	}

	std::size_t Chunk::ConstantInstructionLong(std::string_view name, std::size_t offset) {

		Byte bytes[] = { m_bytes[offset + 1],  m_bytes[offset + 2] };
		std::size_t addr;
		std::memcpy(&addr, bytes, sizeof(std::size_t));
		std::printf("%-16s %4zu '", name.data(), addr);
		Memory::PrintValue(m_memory.GetHandle()[addr], false, true);
		std::printf("'\n");
		return offset + 3;
	}

	void Chunk::Write8(const Byte& byte) {

		m_bytes.push_back(byte);
		m_lines.push_back(m_current_line);
	}

	void Chunk::Write16(const Byte& byte1, const Byte& byte2) {

		Write8(byte1);
		Write8(byte2);
	}

	void Chunk::WriteConstantLong(const Ref<Common::Value>& value) {

		std::size_t addr = AddConstant(value);
		
        Byte bytes[2];
        bytes[0] = (addr >> 8) & 0xFF;
        bytes[1] = (addr >> 0) & 0xFF;

		Write8(OpCode::ConstantLong);
		Write16(bytes[0], bytes[1]);
	}

	void Chunk::WriteConstant(const Ref<Common::Value>& value) {
		Write16(OpCode::Constant, AddConstant(value));
	}

	std::size_t Chunk::AddConstant(const Ref<Common::Value>& value) {
		m_memory.Write(value);
		return m_memory.Size() - 1;
	}

    void Chunk::Free() {
        m_memory.Free();
        m_bytes.clear();
    }

    void Chunk::Add(const std::string& key, Ref<Common::Value> value) {
        m_values[key] = std::move(value);
    }

    Ref<Common::Value> Chunk::Get(const std::string &key) {
        return m_values[key];
    }

    Memory& Chunk::GetMemory() {
        return m_memory;
    }


}
