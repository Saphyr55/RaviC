#pragma once

#include <vector>
#include <set>
#include <string>
#include <map>
#include "common/common.hpp"
#include "vm/memory.hpp"

namespace VM {

class RVM;

class Chunk {

public:
	std::size_t AddConstant(const Common::Value& value);
    void Add(const std::string& key, Ref<Common::Value> value);
    Ref<Common::Value> Get(const std::string& key);
	std::size_t Disassemble(std::size_t offset);
	void Disassemble(std::string_view name);
	void Write8(const Byte& byte);
	void Write16(const Byte& byte1, const Byte& byte2);
	void WriteConstantLong(const Common::Value& value);
	void WriteConstant(const Common::Value& value);
	inline void SetLine(std::size_t line) { m_current_line = line; }
    void Free();

public:
	Chunk() = default;
	Chunk(const Chunk&) = default;
	Chunk(Chunk&&) = default;
	~Chunk() = default;

private:
	static std::size_t SimpleInstruction(std::string_view name, std::size_t offset);
	std::size_t ConstantInstruction(std::string_view name, std::size_t offset);
	std::size_t ConstantInstructionLong(std::string_view name, std::size_t offset);

private:
    std::map<std::string, Ref<Common::Value>> m_values;
    Ref<Chunk> m_enclosing;
	Memory m_memory;
	std::size_t m_current_line = 0;
	std::vector<std::uint32_t> m_lines;
	std::vector<Byte> m_bytes;

	friend class RVM;
};

}