#include "vm/memory.hpp"

#include <cstring>

namespace VM {

	void Memory::PrintValue(const Value& value) {
		std::printf("%g", value);
	}

	void Memory::Write(const Value& value) {
		m_values.push_back(value);
	}

	std::size_t Memory::Size() const {
		return m_values.size();
	}

	std::vector<Value>& Memory::GetHandle() {
		return m_values;
	}

}