#pragma once

#include <list>
#include <string>
#include "common/common.hpp"
#include "common/let.hpp"

namespace Common {

enum class ValType : std::size_t {
    TNull,
    TChar,
    TFloat32,
    TFloat64,
    TInt32,
    TUint32,
    TInt64,
    TUint64,
    TString,
    TUnit,
    TFunction,
    TBoolean,
};

class Value
{
public:
    static Ref<Value> OfLet(Obj::Let let);
    static Ref<Value> OfString(std::string str);
    static Ref<Value> OfFloat64(double number);
    static Ref<Value> OfBoolean(bool val);
    static Ref<Value> OfTrue();
    static Ref<Value> OfFalse();
    static Ref<Value> OfNull();

public:
    bool operator==(Ref<Value> v);
    inline ValType Type() const { return m_type; };
    bool Is(ValType type) const;
    bool IsString() const;
    bool IsBoolean() const;
    bool IsNumber() const;
    virtual std::string ToString() const = 0;

public:
    Value(ValType type) : m_type(type) { };
    virtual ~Value() = default;

protected:
	ValType m_type;
};

template<class T>
class VPrimitiveBase : public Value {
public:
    inline T Get() const { return m_value; }

public:
    VPrimitiveBase(T val, ValType type) : Value(type), m_value(val) { }
    ~VPrimitiveBase() = default;

protected:
    T m_value;
};

class VNull final : public VPrimitiveBase<any> {
public:
    inline std::string ToString() const { return "null"; }
public:
    VNull(any val) : VPrimitiveBase<any>(val, ValType::TNull) {}
};

class VLet final : public VPrimitiveBase<Obj::Let> {

public:
    inline std::string ToString() const { return "let"; }

public:
    VLet(Obj::Let val) : VPrimitiveBase<Obj::Let>(val, ValType::TUnit) { }

};

class VUnit final : public VPrimitiveBase<any> {
public:
    inline std::string ToString() const { return "unit"; }
};

class VFunction final : public VPrimitiveBase<any> {
public:
    inline std::string ToString() const { return "function"; }
};

class VChar final : public VPrimitiveBase<char> {
public:
    inline std::string ToString() const { return "char"; }
};

class VBoolean final : public VPrimitiveBase<bool> {
public:
    inline std::string ToString() const { return "boolean"; }
public:
    VBoolean(bool val) : VPrimitiveBase<bool>(val, ValType::TBoolean) {}
};

class VString final : public VPrimitiveBase<std::string> {
public:
    inline std::string ToString() const { return "string"; }

public:
    VString(std::string val) : VPrimitiveBase<std::string>(val, ValType::TString) {}
};

template<typename T>
class VNumber : public VPrimitiveBase<T> {

public:
    virtual inline std::string ToString() const { return "number"; }

public:
    VNumber(T number, ValType type): VPrimitiveBase<T>(number, type) { }
};

class VFloat32 final : public VNumber<float> {
public:
    inline std::string ToString() const { return "float32"; }
};

class VFloat64 final : public VNumber<double> {

public:
    inline std::string ToString() const { return "float64"; }

public:
    VFloat64(double number) : VNumber<double>(number, ValType::TFloat64) {}
};

class VInt32 final : public VNumber<std::int32_t> {
public:
    inline std::string ToString() const { return "int32"; }
};

class VInt64 final : public VNumber<std::int64_t> {
public:
    inline std::string ToString() const { return "int64"; }
};

class VUint64 final : public VNumber<std::uint64_t> {
public:
    inline std::string ToString() const { return "uint64"; }
};

class VUint32 final : public VNumber<std::uint32_t> {
public:
    inline std::string ToString() const { return "uint32"; }
};

}
