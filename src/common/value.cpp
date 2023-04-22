#include "common/value.hpp"

namespace Common {

    bool Value::Is(ValType type) const {
        return m_type == type;
    }

    Ref<Value> Value::OfString(std::string str) {
        return std::make_shared<VString>(str);
    }

    Ref<Value> Value::OfFloat64(double number) {
        return std::make_shared<VFloat64>(number);
    }

    bool Value::IsNumber() const {
        return
            Is(ValType::TUint64)  ||
            Is(ValType::TUint32)  ||
            Is(ValType::TFloat64) ||
            Is(ValType::TFloat32) ||
            Is(ValType::TInt32)   ||
            Is(ValType::TInt64)  ;
    }

    Ref<Value> Value::OfTrue() {
        return OfBoolean(true);
    }

    Ref<Value> Value::OfFalse() {
        return OfBoolean(false);
    }

    Ref<Value> Value::OfNull() {
        return std::make_shared<VNull>(nullptr);
    }

    Ref<Value> Value::OfBoolean(bool val) {
        return std::make_shared<VBoolean>(val);
    }

    bool Value::operator==(Ref<Value> v1) {
        if (!v1->Is(Type())) return false;
        switch (v1->Type()) {
            case ValType::TUnit:
            case ValType::TNull:
                return true;
            case ValType::TBoolean:
                return Cast<VBoolean>(v1)->Get() == Cast<VBoolean>(std::shared_ptr<Value>(this))->Get();
            case ValType::TChar:
                return Cast<VChar>(v1)->Get() == Cast<VChar>(std::shared_ptr<Value>(this))->Get();
            case ValType::TFloat32:
                return Cast<VFloat32>(v1)->Get() == Cast<VFloat32>(std::shared_ptr<Value>(this))->Get();
            case ValType::TFloat64:
                return Cast<VFloat64>(v1)->Get() == Cast<VFloat64>(std::shared_ptr<Value>(this))->Get();
            case ValType::TInt32:
                return Cast<VInt32>(v1)->Get() == Cast<VInt32>(std::shared_ptr<Value>(this))->Get();
            case ValType::TUint32:
                return Cast<VUint32>(v1)->Get() == Cast<VUint32>(std::shared_ptr<Value>(this))->Get();
            case ValType::TInt64:
                return Cast<VInt64>(v1)->Get() == Cast<VInt64>(std::shared_ptr<Value>(this))->Get();
            case ValType::TUint64:
                return Cast<VUint64>(v1)->Get() == Cast<VUint64>(std::shared_ptr<Value>(this))->Get();
            case ValType::TString:
            case ValType::TFunction:
                return v1 == std::shared_ptr<Value>(this);
        }
        return false;
    }

    bool Value::IsBoolean() const {
        return Is(ValType::TBoolean);
    }

    bool Value::IsString() const {
        return Is(ValType::TString);
    }

    Ref<Value> Value::OfLet(Obj::Let let) {
        return std::make_shared<VLet>(let);
    }

    template class VPrimitiveBase<Obj::Let>;
    template class VPrimitiveBase<std::string>;
    template class VPrimitiveBase<bool>;
    template class VPrimitiveBase<char>;
    template class VPrimitiveBase<any>;
    template class VNumber<float>;
    template class VNumber<double>;
    template class VNumber<std::int32_t>;
    template class VNumber<std::uint64_t>;
    template class VNumber<std::uint32_t>;
    template class VNumber<std::int64_t>;

}