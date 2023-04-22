#include "vm/memory.hpp"

#include <iostream>
#include <algorithm>

namespace VM {

	void Memory::PrintValue(const Ref<Common::Value> value, bool ln, bool isDebug) {
        switch (value->Type()) {
            case Common::ValType::TBoolean:
                std::cout << Cast<Common::VBoolean>(value)->Get();
                break;
            case Common::ValType::TFloat32:
                std::cout << Cast<Common::VFloat32>(value)->Get();
                break;
            case Common::ValType::TFloat64:
                std::cout << Cast<Common::VFloat64>(value)->Get();
                break;
            case Common::ValType::TInt32:
                std::cout << Cast<Common::VInt32>(value)->Get();
                break;
            case Common::ValType::TUint32:
                std::cout << Cast<Common::VUint32>(value)->Get();
                break;
            case Common::ValType::TInt64:
                std::cout << Cast<Common::VInt64>(value)->Get();
                break;
            case Common::ValType::TUint64:
                std::cout << Cast<Common::VUint64>(value)->Get();
                break;
            case Common::ValType::TString: {
                auto str = std::string(Cast<Common::VString>(value)->Get());
                if (isDebug) str.erase(std::remove(str.begin(), str.end(), '\n'), str.cend());
                std::cout << str;
                break;
            }
            case Common::ValType::TFunction:
            case Common::ValType::TChar:
            case Common::ValType::TNull:
            case Common::ValType::TUnit:
                std::cout << value->ToString();
                break;
        }
		if (ln) std::cout << "\n";
	}

	void Memory::Write(const Ref<Common::Value> value) {
		m_values.push_back(value);
	}

	std::size_t Memory::Size() const {
		return m_values.size();
	}

	std::vector<Ref<Common::Value>>& Memory::GetHandle() {
		return m_values;
	}

    Ref<Common::Value> Memory::Pop() {
        auto item = m_values.back();
        m_values.pop_back();
        return item;
    }

    Ref<Common::Value> Memory::Peek(const std::size_t& distance) {
        return Get(m_values.size() - 1 - distance);
    }

    void Memory::Free() {
        m_values.clear();
    }

    Ref<Common::Value> Memory::Get(const std::size_t& address) {
        return m_values[address];
    }

}