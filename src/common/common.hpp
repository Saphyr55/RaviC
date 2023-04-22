#pragma once

#include <memory>
#include <cstdint>

template<typename T>
using Ref = std::shared_ptr<T>;
using Byte = std::uint8_t;
using any = void*;

template<typename Base, typename T>
inline bool instance_of(const T*) {
    return std::is_base_of<Base, T>::value;
}

template<typename V, typename Base>
static Ref<V> Cast(Ref<Base> v) {
    return std::dynamic_pointer_cast<V>(v);
}