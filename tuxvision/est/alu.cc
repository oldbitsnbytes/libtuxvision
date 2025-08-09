/******************************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                                        *
 *   serge.lussier@oldlonecoder.club                                                      *
 *                                                                                        *
 *                                                                                        *
 *   Unless otherwise specified, all code in this project is written                      *
 *   by the author (Serge Lussier)                                                        *
 *   and no one else then not even {copilot, chatgpt, or any other AI calamities}         *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge Lussier also apply                      *
 *   Open source FREE licences also apply To the code from the same author (Serge Lussier)*
 *   ----------------------------------------------------------------------------------   *
 ******************************************************************************************/

#include <tuxvision/est/alu.h>


namespace tux::lang::est
{

alu   alu::void_alu{};

#define debug_out ;
//book::debug() << book::fn::function << "rhs=" << rhs << ", this = " << v.d << " : types: " << lang::component::type_name(T);


alu::alu(int8_t rhs) : T(lang::type::I8|lang::type::Number|lang::type::Leaf)
{ v.d = rhs; debug_out}
alu::alu(int16_t rhs) : T(lang::type::I16|lang::type::Number|lang::type::Leaf)
{ v.d = rhs; debug_out}
alu::alu(int32_t rhs) : T(lang::type::I32|lang::type::Number|lang::type::Leaf)
{ v.d = rhs; debug_out}
alu::alu(int64_t rhs) : T(lang::type::I64|lang::type::Number|lang::type::Leaf)
{ v.d = rhs; debug_out}
alu::alu(uint8_t rhs) : T(lang::type::U8|lang::type::Number|lang::type::Leaf)
{ v.d = rhs; debug_out}
alu::alu(uint16_t rhs) : T(lang::type::U16|lang::type::Number|lang::type::Leaf)
{ v.d = rhs; debug_out}
alu::alu(uint32_t rhs) : T(lang::type::U32|lang::type::Number|lang::type::Leaf)
{ v.d = rhs; debug_out}
alu::alu(uint64_t rhs) : T(lang::type::U64|lang::type::Number|lang::type::Leaf)
{ v.d = rhs; debug_out}
alu::alu(bool rhs) : T(lang::type::Bool)
{v.d = rhs; debug_out}

#define cast(rhs)\
(rhs).T & lang::type::Bool  ? (rhs).v.d != false :\
(rhs).T & lang::type::I8       ? (rhs).v.d:\
(rhs).T & lang::type::I16      ? (rhs).v.d:\
(rhs).T & lang::type::I32      ? (rhs).v.d:\
(rhs).T & lang::type::I64      ? (rhs).v.d:\
(rhs).T & lang::type::U8       ? (rhs).v.d:\
(rhs).T & lang::type::U16      ? (rhs).v.d:\
(rhs).T & lang::type::U32      ? (rhs).v.d:\
(rhs).T & lang::type::U64      ? (rhs).v.d:\
(rhs).T & lang::type::Float    ? (rhs).v.d :0




alu::alu(double rhs)
{
    T = lang::type::Number | lang::type::Float | lang::type::Leaf;
    v.d = rhs;
    auto l = sys::debug(1); l << debug_infos() << l;
}

alu::alu() {;}

alu::alu(float rhs)
{
    T = lang::type::Number | lang::type::Float | lang::type::Leaf;
    v.d = (double)rhs;
}

alu::alu(const std::string& rhs)
{
    T = lang::type::Text | lang::type::Leaf;
    v.t = &text; text = rhs;
}

alu::alu(const char* rhs)
{

    T = lang::type::Text | lang::type::Leaf;
    v.t = &text; text = rhs;
}

alu::alu(void* rhs)
{
    T = lang::type::Any | lang::type::VoidPtr;
    v.p = rhs;
}

bool alu::type_size(const alu& Rhs_) const
{
    lang::type::value me = T & (lang::type::I8 | lang::type::I16 | lang::type::I32 | lang::type::I64 | lang::type::U8 | lang::type::U16 | lang::type::U32 | lang::type::U64 | lang::type::Float);
    lang::type::value r = Rhs_.T & (lang::type::I8 | lang::type::I16 | lang::type::I32 | lang::type::I64 | lang::type::U8 | lang::type::U16 | lang::type::U32 | lang::type::U64 |lang::type:: Float);
    return static_cast<U64>(me) - static_cast<U64>(r) >= 0;
}

alu::operator bool() const
{
    if (!static_cast<U64>(v.d) || !static_cast<U64>(v.d))
        return false;
    if (!T)
        return false;
    if (T & lang::type::Text)
        return !text.empty();
    if (T & lang::type::Number)
        return v.d != 0.0f;
    if (T & lang::type::VoidPtr)
        return value<void *>() != nullptr;
    return false;
}

//#define lr_number(rhs) if ((T&lang::type::Number) && ((rhs).T&lang::type::Number))
#define lr_number(rhs) if (T&lang::type::Number&(rhs).T)
#define lrtext(rhs)   if ((T&lang::type::Text) && ((rhs).T&lang::type::Text))
#define lr_any(rhs)    if ((T&lang::type::Any)  && ((rhs).T&lang::type::Any))
#define is_integrale(rhs) if (!((rhs).T&lang::type::Float))
#define istext(i)    if((i).T & lang::type::Text)
#define is_number if ((T&(lang::type::Float|lang::type::Number)))
#define throw_on_binary_op(rhs)  \
    throw sys::exception()[ \
        sys::error(1) <<" Incompatible binary operation between " \
        << lang::component::type_name(T) << " and " << lang::component::type_name((rhs).T)];

// #define delstring_if(rhs) if ((T&lang::type::Text) && (!(rhs).T & lang::type::Text)) delete v..t;  Vieux stuff

alu& alu::operator=(const alu& rhs)
{
    debug_out
    if (&rhs != this)
    {
        T = rhs.T;
        v = rhs.v;
        return *this; // WTF???????
    }

    if (T & lang::type::Text)
    {
        text = rhs.text;
        return *this;
    }
    if (T & lang::type::Any)
    {
        v.p = rhs.v.p;
        return *this;
    }
    *this = false;
    return *this;
}


alu alu::operator+(const alu& rhs) const
{
    lrtext(rhs)
        return {text + rhs.text};
    else lr_number(rhs)
        return {v.d + rhs.v.d}; // Slow?

    throw_on_binary_op(rhs)

    return {false};
}


alu alu::operator==(const alu & lf) const
{
    alu a;
    lrtext(lf)
    {
        a = lf.text == text;
        return a;
    }
    lr_number(lf)
    {
        return lf.number<double>() == number<double>();
    }
    throw_on_binary_op(lf)
    return {false};
}

alu alu::operator[](alu::CNV c) const
{
    istext(*this)
    {
        return {0.0f};
    }
    auto d = v.d;

    alu    a;
    switch (c)
    {
        case alu::DEG:
            a = d * 180.0f / 3.141592653589793;
            break;
        case alu::RAD:
            a = d * 3.141592653589793 / 180.0f;
            break;
        default:
            a = 0.0f;
            break;
    }
    return a;
}

alu alu::operator-(const alu& rhs)
{
    //book::debug() << book::fn::function << " rhs=" << rhs.to_str();
    lrtext(rhs)
    {
        throw_on_binary_op(rhs)
        //return remove_substr_copy(rhs.text);
        return *this;
    }

    lr_number(rhs)
        return {v.d - rhs.v.d};
    lr_any(rhs)
        return {reinterpret_cast<uint64_t>(value<void *>()) - reinterpret_cast<uint64_t>(rhs.value<void *>())};

    throw_on_binary_op(rhs)
    return {false};
}


alu alu::operator*(const alu& rhs) const
{
    alu f;
    lrtext(rhs)
    {
        std::size_t pos = rhs.text.find(text);
        return {pos != std::string::npos};
    }

    //else
    lr_number(rhs)
        return {v.d * rhs.v.d};

    throw_on_binary_op(rhs)
    return {};
}

alu alu::operator/(const alu& rhs) const
{
    lrtext(rhs)
    {
        throw_on_binary_op(rhs)
        return {};
    }
    //    f = text + rhs.text;
    //else
    lr_number(rhs)
    {
        double vr = rhs.v.d;
        double lr = v.d;

        if (vr == 0.0L)
        {
            throw sys::exception()[ sys::error(1) << " alu operation: " << rem::code::divbyzero << " =>(" << (*this)() << " / " << rhs() << ")" ];
            return "Inf";
        }
        return {lr / vr};
    }

    throw_on_binary_op(rhs)
    return {};
}

alu alu::operator%(const alu& rhs) const
{
    alu f;
    lrtext(rhs)
    {
        throw_on_binary_op(rhs)
        return {};
    }
    lr_number(rhs)
        return {static_cast<U64>(v.d) % static_cast<U64>(rhs.v.d)};

    throw_on_binary_op(rhs)
    return {};
}

alu alu::operator^(const alu& rhs) const
{
    alu f;
    lrtext(rhs)
    {
        throw_on_binary_op(rhs)
        return {};
    }

    lr_number(rhs)
        return {static_cast<U64>(v.d) ^ static_cast<U64>(rhs.v.d)};

    throw_on_binary_op(rhs)
    return {};
}

alu alu::operator<<(const alu& rhs) const
{
    alu f;
    lrtext(rhs)
    {
        tux::string str = text;
        str << rhs.text;
        return {str()};
    }

    lr_number(rhs)
        return {static_cast<U64>(v.d)<< static_cast<U64>(rhs.v.d)};

    throw_on_binary_op(rhs)
    return f;
}



alu alu::operator>>(const alu& rhs) const
{
    alu f;
    lrtext(rhs)
    {
        throw_on_binary_op(rhs)
    }
    lr_number(rhs)
        return {static_cast<U64>(v.d) >> static_cast<U64>(rhs.v.d)};


    throw_on_binary_op(rhs)
    return f;
}

alu alu::operator|(const alu& rhs) const
{

    lrtext(rhs)
    {
        return {text + rhs.text};
    }
    lr_number(rhs)
        return {static_cast<U64>(v.d) | static_cast<U64>(rhs.v.d)};

    throw_on_binary_op(rhs)
    return {};
}

alu alu::operator&(const alu& rhs) const
{
    lrtext(rhs)
    {
        throw_on_binary_op(rhs)
    }
    lr_number(rhs)
        return {static_cast<U64>(v.d) & static_cast<U64>(rhs.v.d)};

    throw_on_binary_op(rhs)
    return {};
}

alu alu::operator~() const
{
    istext(*this)
        throw sys::exception()[ sys::error(1) <<"cannot execute bitwise 1's complement operation on string."];


    is_number
    {
        if (T & lang::type::Float)
            return {1 / v.d};
        return {~static_cast<U64>(v.d)};
    }

    throw sys::exception()[ sys::error(1) <<"cannot execute bitwise 1's complement on " << lang::component::type_name(T) ];
    return {};
}

// assign operators:
alu& alu::operator+=(const alu& rhs)
{
    lrtext(rhs)
    {
        text = text + rhs.text;
        return *this;
    }

    lr_number(rhs)
    {
        v.d += rhs.v.d;
        return *this;
    }

    throw_on_binary_op(rhs)
    return *this;
}

alu& alu::operator-=(const alu& rhs)
{
    lrtext(rhs) return *this;
    //return remove_substr(rhs.text);

    lr_number(rhs)
    {
        v.d  -= rhs.v.d;
        return *this;
    }

    throw_on_binary_op(rhs)
    return *this;
}

alu& alu::operator*=(const alu& rhs)
{
    lrtext(rhs)
    {
        throw_on_binary_op(rhs)
        return *this;
    }

    lr_number(rhs)
    {
        v.d *= rhs.v.d;
        return *this;
    }

    throw_on_binary_op(rhs)
    return *this;
}

alu& alu::operator/=(const alu& rhs)
{
    lrtext(rhs)
    {
        throw_on_binary_op(rhs)
        return *this;
    }

    lr_number(rhs)
    {

        if (v.d==0.0f)
        {
            throw sys::exception()[ sys::error(1) <<rem::code::divbyzero]; // Oops plutôt erreur fatale!!
            v.d = 0.0;
            T = lang::type::Null;
            return *this;
        }
        v.d /= rhs.v.d;
        return *this;
    }
    throw_on_binary_op(rhs)
    return *this;
}

alu::~alu(){;}

alu& alu::operator%=(const alu& rhs)
{
    lrtext(rhs) return *this;
    //return completely_remove_substr(rhs.text);

    lr_number(rhs)
    {
        u64 u = v.d;
        v.d = u %= static_cast<U64>(rhs.v.d);
        T &= ~lang::type::Float;
    }
    throw_on_binary_op(rhs)
    return *this;
}

alu& alu::operator|=(const alu& rhs)
{

    lrtext(rhs)
    {
        throw_on_binary_op(rhs)
        return *this;
    }

    lr_number(rhs)
    {
        v.d = static_cast<U64>(v.d) | static_cast<U64>(rhs.v.d);
        return *this;
    }
    throw_on_binary_op(rhs)
    return *this;
}

alu& alu::operator&=(const alu& rhs)
{
    lrtext(rhs)
    {
        throw_on_binary_op(rhs)
        return *this;
    }
    //    a = std::string(text + rhs.text);
    //else
    lr_number(rhs)
    {
        v.d  = static_cast<U64>(v.d) &  static_cast<U64>(rhs.v.d);
        T &= ~lang::type::Float;
        return *this;
    }
    throw_on_binary_op(rhs)
    return *this;
}

alu& alu::operator^=(const alu& rhs)
{
    lrtext(rhs)
    {
        throw_on_binary_op(rhs)
        return *this;
    }

    lr_number(rhs)
    {
        /// @note xor ?
        v.d = static_cast<U64>(v.d) ^ static_cast<U64>(rhs.v.d);
        T &= ~lang::type::Float;
        return *this;
    }

    throw_on_binary_op(rhs)
    return *this;
}

// boolean operators:
alu alu::operator>(const alu& rhs)
{
    lrtext(rhs)
        return {text > rhs.text};

    lr_number(rhs)
        return {v.d > rhs.v.d};

    throw_on_binary_op(rhs)
    return {false};
}

alu alu::operator<(const alu& rhs)
{
    lrtext(rhs)
        return {text < rhs.text};

    lr_number(rhs)
        return {v.d < rhs.v.d};

    throw_on_binary_op(rhs)
    return {false};

}

alu alu::operator>=(const alu& rhs)
{
    lrtext(rhs)
        return {text >= rhs.text};

    lr_number(rhs)
        return {v.d >= rhs.v.d};

    throw_on_binary_op(rhs)
    return {false};

}

alu alu::operator<=(const alu& rhs)
{
    lrtext(rhs)
        return {text <= rhs.text};

    lr_number(rhs)
        return {v.d <= rhs.v.d};

    throw_on_binary_op(rhs)
    return {false};

}

alu alu::operator!=(const alu& rhs)
{
    lrtext(rhs)
        return {text != rhs.text};

    lr_number(rhs)
        return {v.d != rhs.v.d};

    throw_on_binary_op(rhs)
    return {false};
}

alu alu::operator||(const alu& rhs)
{
    lrtext(rhs)
        return {!text.empty() || (!rhs.text.empty())};

    lr_number(rhs)
        return {v.d || rhs.v.d};

    throw_on_binary_op(rhs)
    return {false};
}

alu alu::operator&&(const alu& rhs)
{
    lrtext(rhs)
        return {!text.empty() && (!rhs.text.empty())};

    lr_number(rhs)
        return {v.d && rhs.v.d};

    throw_on_binary_op(rhs)
    return {false};
}

bool alu::operator!()
{
    return v.d ==0.0f;
}

alu alu::operator-()
{
    istext(*this)
    {
        throw sys::exception()[ sys::error(1) <<"cannot execute sign operator on string -(\"" << (*this)() << "\")."];
    }
    return -v.d;
}

alu alu::operator+()
{
    istext(*this)
    {
        throw sys::exception()[  sys::error(1) <<"cannot execute sign operator on string +(\"" << (*this)() << "\")."];
    }

    is_number
        if (v.d < 0.0f)
            return {v.d * -1};

    throw sys::exception()[ sys::error(1) <<"cannot execute relational operations on " << lang::component::type_name(T)];
    return {false};
}

alu alu::operator++(int)
{

    istext(*this)
    {
        throw sys::exception()[ sys::error(1) <<"cannot increment string (\"" << (*this)() << "\")++."];
    }

    is_number
    {
        auto f = v.d;
        alu    ff(f);
        ++f;
        v.d = f;
        return ff;
    }
    throw sys::exception()[ sys::error(1) <<"cannot execute increment operations on " << lang::component::type_name(T)];
    return {false};
}

alu alu::operator++()
{
    istext(*this)
    {
        throw sys::exception()[ sys::error(1) <<"cannot increment string ++(\"" << (*this)() << "\")."];
    }

    is_number
    {
        auto f = v.d;
        ++f;
        v.d = f;
        return *this;
    }
    throw sys::exception()[ sys::error(1) <<"cannot execute increment operations on " << lang::component::type_name(T)];
    return {false};
}

alu alu::operator--()
{

    istext(*this)
    {
        throw sys::exception()[ sys::error(1) <<"cannot decrement string --(\"" << (*this)() << "\")."];
    }

    is_number
    {
        auto f = v.d;
        ++f;
        v.d = f;
        return *this;
    }
    throw sys::exception()[ sys::error(1) <<"cannot execute decrement operations on " << lang::component::type_name(T)];
    return {false};
}

alu alu::operator--(int)
{
    istext(*this)
    {
        throw sys::exception()[ sys::error(1) <<"cannot decrement string (\"" << (*this)() << "\")++."]; // oops... What if long text string?
    }

    is_number
    {
        auto f = v.d;
        alu    ff(f);
        ++f;
        v.d = f;
        return ff;
    }
    throw sys::exception()[ sys::error(1) <<"cannot execute decrement operations on " << lang::component::type_name(T)];
    return {false};
}

alu alu::radical(const alu& rhs) const
{
    alu al;
    lr_number(rhs)
    {
        al = alu(std::pow(v.d, 1 / rhs.v.d));
        return al;
    }

    throw sys::exception()[ sys::error(1) <<"cannot execute radical operator on " << lang::component::type_name(T)];
    return {false};
}

//    alu alu::factorial(const alu& Lhs_) const
//    {
//        Book::debug() << ui::color::Yellow << (*this)() << ui::color::Reset << ":" ;
//        lr_number(Lhs_)
//        {
//            int fac = 1;
//            int N = Lhs_.number<int>();
//            if (N >= 10)
//                return {};
//            for (int j = 1; j <= N; j++)
//                fac *= j;
//            Book::debug() << ui::color::Yellow << (*this)() << ui::color::Reset << ":" ;
//            return {fac};
//        }
//        throw book::exception()[ book::except() <<"cannot execute factorial operator on " << lang::component::type_name(T) ;
//        return {false};
//    }

alu alu::factorial() const
{
    // book::debug() << book::fn::function << ui::color::yellow << (*this)() <<ui::color::white << ":"  ;
    lr_number(*this)
    {
        if(v.d < 0)
        {
            throw sys::exception()[ sys::error(1) <<" Invalid Base number :" << ui::color::yellow << (*this)() << ui::color::reset] ;
            return {};
        }
        int fac = 1;
        int N = number<int>();
        if (N >= 10)
        {
            throw sys::exception()[ sys::except() <<" Base number too high." << ui::color::yellow << (*this)() << ui::color::reset];
            return {};
        }
        for (int j = 1; j <= N; j++)
            fac *= j;
        return { fac };
    }
    throw sys::exception()[ sys::error(1) <<"cannot execute factorial operator on " << lang::component::type_name(T)];
    return { false };

}

alu& alu::operator<<=(const alu& rhs)
{

    lrtext(rhs)
    {
        throw_on_binary_op(rhs)
        return *this;
    }

    lr_number(rhs)
    {
        v.d  = static_cast<integers::u64>(v.d) << static_cast<integers::u64>(rhs.v.d);
        return *this;
    }

    throw_on_binary_op(rhs)
    return *this;
}

alu& alu::operator>>=(const alu& rhs)
{

    lrtext(rhs)
    {
        throw_on_binary_op(rhs)
        return *this;
    }

    lr_number(rhs)
    {
        v.d = static_cast<integers::u64>(v.d) >> static_cast<integers::u64>(rhs.v.d);;
        return *this;
    }

    throw_on_binary_op(rhs)
    return *this;
}




[[maybe_unused]] std::string alu::to_str() const
{
    tux::string str;
    if (T & lang::type::Bool)
    {
        str << (v.d ? "true" : "false");
        return str();
    }

    if (T & lang::type::Number)
    {
        str << v.d;
        return str();
    }

    if (T & lang::type::Text)
        return text;
    if (T & lang::type::VoidPtr)
    {
        str = "@\\{p}";
        str << v.p;
        return str();
    }

    if (T & lang::type::Null)
        str << "null";
    return str();
}


std::string alu::debug_infos()
{
    tux::string str;
    str << "type(s): " << lang::component::type_name(T) << " ";
    str << "value: " << to_str();
    return str();
}


alu::operator std::string() const
{
    return to_str();
}


}
