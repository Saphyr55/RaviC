#pragma once

#include "common/object.hpp"
#include "common/common.hpp"

namespace Common {

struct Value
{
public:
    enum class Type {
        TBool,
        TNull,
        TObject,
        TNumber,
    };

public:
    [[nodiscard]] Object::Type ObjectType() const;
    [[nodiscard]] ObjectPtr AsObject() const;
    [[nodiscard]] StringObjectPtr AsString() const;
    [[nodiscard]] bool AsBoolean() const;
    [[nodiscard]] double AsNumber() const;
	[[nodiscard]] bool IsNull() const;
	[[nodiscard]] bool IsNumber() const;
    [[nodiscard]] bool IsObject() const;
    [[nodiscard]] bool IsString() const;
    [[nodiscard]] bool IsBool() const;
    [[nodiscard]] bool IsEqual(const Value& value) const;

public:
	Value();
    Value(std::string_view value);
    Value(bool value);
	Value(double value);
    ~Value() = default;

public:
	Type VType;
	union {
        ObjectPtr Object;
		bool Boolean;
		double Number;
	} As{};

};

} 
