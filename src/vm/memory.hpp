#pragma once

#include <vector>
#include "common/common.hpp"
#include "common/value.hpp"

namespace VM {

class Memory {

public:
	static void PrintValue(const Common::Value& value, bool ln = false, bool isDebug = false);
    static void PrintObject(const Common::Value &obj, bool isDebug);

public:
    void Write(const Common::Value& value);
    void Free();
    Common::Value Get(const std::size_t& address);
    Common::Value Pop();
    Common::Value Peek(const std::size_t& distance);
	[[nodiscard]] std::size_t Size() const;
	std::vector<Common::Value>& GetHandle();

public:
	Memory() = default;
    Memory(const Memory &mMemory) = default;
	Memory(Memory&&) = default;
    ~Memory() = default;

private:
	std::vector<Common::Value> m_values;

};

}

