#pragma once

#include <vector>
#include "common.hpp"

namespace VM {

class Memory {

public:
	static void PrintValue(const Value& value);
	static void PrintlnValue(const Value& value);
	void Write(const Value& value);
	std::size_t Size() const;
	std::vector<Value>& GetHandle();
	
public:
	Memory() = default;
	Memory(const Memory&) = default;
	Memory(Memory&&) = default;
	~Memory() = default;

private:
	std::vector<Value> m_values;
};

}

