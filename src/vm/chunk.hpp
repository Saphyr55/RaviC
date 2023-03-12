#pragma once

#include <vector>
#include <set>
#include <string>
#include "common.hpp"
#include "memory.hpp"

namespace VM {

class RVM;

class Chunk {

public:
	std::size_t AddConstant(const Value& value);
	std::size_t Disassemble(std::size_t offset);
	void Disassemble(std::string_view name);
	void Write(const Byte& byte);
	void WriteConstant(const Value& value);

public:
	Chunk() = default;
	Chunk(const Chunk&) = default;
	Chunk(Chunk&&) = default;
	~Chunk() = default;

private:
	std::size_t SimpleInstruction(std::string_view name, std::size_t offset);
	std::size_t ConstantInstruction(std::string_view name, std::size_t offset);

private:
	Memory m_memory;
	std::uint32_t m_current_line = 0;
	std::vector<std::uint32_t> m_lines;
	std::vector<Byte> m_bytes;
	
	friend class RVM;
};

}