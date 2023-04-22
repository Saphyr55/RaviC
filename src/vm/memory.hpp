#pragma once

#include <vector>
#include "common/value.hpp"

namespace VM {

class Memory {

public:
	static void PrintValue(const Ref<Common::Value> value, bool ln = false, bool isDebug = false);

public:
    void Write(const Ref<Common::Value> value);
    void Free();
    Ref<Common::Value> Get(const std::size_t& address);
    Ref<Common::Value> Pop();
    Ref<Common::Value> Peek(const std::size_t& distance = 0);
	[[nodiscard]] std::size_t Size() const;
	std::vector<Ref<Common::Value>>& GetHandle();

public:
	Memory() = default;
    Memory(const Memory &mMemory) = default;
	Memory(Memory&&) = default;
    ~Memory() = default;

private:
	std::vector<Ref<Common::Value>> m_values;

};

}

