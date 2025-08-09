//
// Created by oldlonecoder on 25-06-24.
//

#pragma once
#include <tuxc++/est/alu.h>



template<typename oftype> oftype alu::value() const
{

    if constexpr ((std::is_same_v<oftype, std::string>) || (std::is_same_v<oftype, const std::string&>))
        return (value_type & lang::type::Text ? std::any_cast<std::string>(data) : std::string{});
    else
        if constexpr (std::is_same_v<oftype, const char*>)
            return (value_type & lang::type::Text ? std::any_cast<std::string>(data).c_str() : nullptr);
        else
            if constexpr (std::is_same_v<oftype, void*>)
                return std::any_cast<void*>(data);
            else
                return static_cast<oftype>(
                        value_type & lang::type::I8 ?    std::any_cast<i8>(data) :
                        value_type & lang::type::I16 ?   std::any_cast<i16>(data) :
                        value_type & lang::type::I32 ?   std::any_cast<i32>(data) :
                        value_type & lang::type::I64 ?   std::any_cast<i64>(data) :
                        value_type & lang::type::U8 ?    std::any_cast<u8>(data) :
                        value_type & lang::type::U16 ?   std::any_cast<u16>(data) :
                        value_type & lang::type::U32 ?   std::any_cast<u32>(data) :
                        value_type & lang::type::U64 ?   std::any_cast<u64>(data) :
                        value_type & lang::type::Bool ?  std::any_cast<bool>(data) :
                        value_type & lang::type::Float ? std::any_cast<float>(data) : 0
                    );

}

template<typename T> alu& alu::operator=(const T& _value)
{
    value_type = infer_type<T>();
    auto l = sys::debug(1);
    l << "infered type: '" << color::yellow << lang::component::type_name(value_type) << color::z << "'" << l;
    if (value_type  &  (lang::type::Natural|lang::type::Integer))
        value_type = scale(_value);
    else
        data = _value; // The other infer_type'd values.

    return *this;
}




template<typename T>
lang::type::value alu::scale(const T&_value)
{
    if constexpr(std::is_same_v<T,const std::string&>) return lang::type::Text;
    if constexpr(std::is_same_v<T,std::string>) return lang::type::Text;
    if constexpr(std::is_same_v<T,const char*>) return lang::type::Text;
    if constexpr(std::is_same_v<T,char*>) return lang::type::Text;

    if ((_value >= std::numeric_limits<int8_t>::lowest())&& (_value <= std::numeric_limits<int8_t>::max()))
    {
        data = static_cast<int8_t>(_value);
        return lang::type::I8;
    }
    if ((_value >= std::numeric_limits<int16_t>::lowest())&&(_value <= std::numeric_limits<int16_t>::max()))
    {
        data = static_cast<int16_t>(_value);
        return lang::type::I16;
    }
    if ((_value >= std::numeric_limits<int32_t>::min())&&(_value <= std::numeric_limits<int32_t>::max()))
    {
        data = static_cast<int32_t>(_value);
        return lang::type::I32;
    }
    if ((_value >= std::numeric_limits<int64_t>::min())&&(_value <= std::numeric_limits<int64_t>::max()))
    {
        data = static_cast<int64_t>(_value);
        return lang::type::I64;
    }
    if ((_value >= std::numeric_limits<uint8_t>::min())&&(_value <= std::numeric_limits<uint8_t>::max()))
    {
        data = static_cast<uint8_t>(_value);
        return lang::type::U8;
    }
    if ((_value >= std::numeric_limits<uint16_t>::min())&&(_value <= std::numeric_limits<uint16_t>::max()))
    {
        data = static_cast<uint16_t>(_value);
        return lang::type::U16;
    }
    if ((_value >= std::numeric_limits<uint32_t>::min())&&(_value <= std::numeric_limits<uint32_t>::max()))
    {
        data = static_cast<uint32_t>(_value);
        return lang::type::U32;
    }
    if ((_value >= std::numeric_limits<uint64_t>::min())&&(_value <= std::numeric_limits<uint64_t>::max()))
    {
        data = static_cast<uint64_t>(_value);
        return lang::type::U64;
    }
    if ((_value >= std::numeric_limits<float>::min())&&(_value <= std::numeric_limits<float>::max()))
    {
        data = static_cast<float>(_value);
        return lang::type::Float;
    }
    if ((_value >= std::numeric_limits<double>::min())&&(_value <= std::numeric_limits<double>::max()))
    {
        data = static_cast<double>(_value);
        return lang::type::F64;
    }
    if ((_value >= std::numeric_limits<long double>::min())&&(_value <= std::numeric_limits<long double>::max()))
    {
        data = static_cast<long double>(_value);
        return lang::type::F128;
    }
    data = static_cast<uint64_t>(_value);
    return lang::type::VoidPtr;
}


template<typename T> lang::type::value alu::infer_type() const
{
    if constexpr (std::is_same_v<T, bool>)
        return lang::type::Bool;
    if constexpr (std::is_same_v<T, char*>)
        return lang::type::Text;
    if constexpr (std::is_same_v<T, const char*>)
        return lang::type::Text;
    if constexpr (std::is_same_v<T, float>)
        return lang::type::Float;
    if constexpr (std::is_same_v<T, int>)
        return lang::type::Integer;
    if constexpr (std::is_same_v<T, double>)
        return lang::type::F64;
    if constexpr (std::is_same_v<T, long double>)
        return lang::type::F128;
    if constexpr (std::is_same_v<T, int8_t>)
        return lang::type::I8;
    if constexpr (std::is_same_v<T, int16_t>)
        return lang::type::I16;
    if constexpr (std::is_same_v<T, int32_t>)
        return lang::type::I32;
    if constexpr (std::is_same_v<T, int64_t>)
        return lang::type::I64;
    if constexpr (std::is_same_v<T, uint8_t>)
        return lang::type::U8;
    if constexpr (std::is_same_v<T, uint16_t>)
        return lang::type::U16;
    if constexpr (std::is_same_v<T, uint32_t>)
        return lang::type::U32;
    if constexpr (std::is_same_v<T, uint64_t>)
        return lang::type::U64;
    if constexpr (std::is_same_v<T, std::string>)
        return lang::type::Text;
    if constexpr (std::is_same_v<T, const std::string&>)
        return lang::type::Text;
    if constexpr (std::is_same_v<T, bool>)
        return lang::type::Bool;
    if constexpr (std::is_same_v<T, void*>)
        return lang::type::VoidPtr;

    return lang::type::VoidPtr;
}
