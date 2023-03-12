#pragma once

#include <vector>
#include "common.hpp"

namespace VM {



enum OpCode : std::size_t {
	Constant,
	Return,
};

class Memory {

public:
	static void PrintValue(const Value& value);
	void Write(const Value& value);
	std::size_t Size() const;
	std::vector<Value>& GetHandle();
	
public:
	Memory() = default;
	Memory(const Memory&) = delete;
	Memory(Memory&&) = delete;
	~Memory() = default;

private:
	std::vector<Value> m_values;
};

}

