#include "common/value.hpp"
#include <memory>
#include <string_view>

namespace Common {

	Value::Value(): VType(Type::TNull) {
		As.Number = 0;
	}

	Value::Value(double value) : VType(Type::TNumber) {
		As.Number = value;
	}

	Value::Value(bool value) : VType(Type::TBool) {
		As.Boolean = value;
	}

	Value::Value(std::string_view value) : VType(Type::TObject){
		As.Object = new StringObject(value);
	}

	bool Value::IsNull() const {
		return VType == Type::TNull;
	}

	bool Value::IsNumber() const {
		return VType == Type::TNumber;
	}

	bool Value::IsBool() const {
		return VType == Type::TBool;
	}

	bool Value::IsObject() const {
		return VType == Type::TObject;
	}

    StringObjectPtr Value::AsString() const {
        return (StringObjectPtr) AsObject();
    }

	ObjectPtr Value::AsObject() const {
		return As.Object;
	}

	double Value::AsNumber() const {
		return As.Number;
	}

	bool Value::AsBoolean() const {
		return As.Boolean;
	}

	Object::Type Value::ObjectType() const {
		return AsObject()->OType;
	}

    bool Value::IsEqual(const Value &value) const {

		if (VType != value.VType) return false;

		switch (value.VType) {
			case Type::TBool:   	return As.Boolean == value.As.Boolean;
			case Type::TNull:    return true;
			case Type::TNumber: 	return As.Number == value.As.Number;
			default:         	return false; // Unreachable.
		}

    }

	bool Value::IsString() const {
		return As.Object->OType == Object::Type::String;
	}

}