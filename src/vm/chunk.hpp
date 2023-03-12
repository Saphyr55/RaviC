#pragma once

#include <vector>
#include <string>
#include "common.hpp"
#include "memory.hpp"

namespace VM {

class Chunk {

public:
	std::size_t AddConstant(const Value& value);
	std::size_t Disassemble(std::size_t offset);
	void Disassemble(std::string_view name);
	void Write(const Byte& byte);

public:
	Chunk() = default;
	Chunk(const Chunk&) = delete;
	Chunk(Chunk&&) = delete;
	~Chunk() = default;

private:
	std::size_t SimpleInstruction(std::string_view name, std::size_t offset);
	std::size_t ConstantInstruction(std::string_view name, std::size_t offset);

private:
	Memory m_memory;
	std::uint32_t m_current_line;
	std::vector<std::uint32_t> m_lines;
	std::vector<Byte> m_bytes;

};

}