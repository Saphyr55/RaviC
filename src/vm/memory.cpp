#include "vm/memory.hpp"

#include <iostream>
#include <algorithm>

namespace VM {

	void Memory::PrintValue(const Common::Value& value, bool ln, bool isDebug) {
		switch (value.VType) {
            case Common::Value::Type::TNumber:
                std::cout << value.As.Number;
                break;
            case Common::Value::Type::TNull:
                std::cout << "null";
                break;
            case Common::Value::Type::TBool:
                std::cout << (value.As.Boolean ? "true" : "false");
                break;
            case Common::Value::Type::TObject:
                PrintObject(value, isDebug);
                break;
            default:
                break;
		}
		if (ln) std::cout << "\n";
	}

    void Memory::PrintObject(const Common::Value &obj, bool isDebug) {
        switch (obj.AsObject()->OType) {
            case Common::Object::Type::String: {
                std::string str = std::string(obj.AsString()->String);
                if (isDebug)
                    str.erase(std::remove(str.begin(), str.end(), '\n'), str.cend());
                std::cout << "\"" << str << "\"";
                break;
            }
            default: break;
        }
    }

	void Memory::Write(const Common::Value& value) {
		m_values.push_back(value);
	}

	std::size_t Memory::Size() const {
		return m_values.size();
	}

	std::vector<Common::Value>& Memory::GetHandle() {
		return m_values;
	}

    Common::Value Memory::Pop() {

        auto item = m_values.back();
        m_values.pop_back();
        return item;
    }

    Common::Value Memory::Peek(const std::size_t& distance) {
        return Get(m_values.size() - 1 - distance);
    }

    void Memory::Free() {
        m_values.clear();
    }

    Common::Value Memory::Get(const std::size_t& address) {
        return m_values[address];
    }

}