/******************************************************************************************
*   Copyright (C) ...,2025,... by Serge luxsier                                          *
*   serge.luxsier@oldbitsnbytes.club / luxsier.serge@gmail.com                            *
*   ----------------------------------------------------------------------------------   *
*   Unless otherwise specified, all Codes ands files in this project is written          *
*   by the author and owned by the auther (Serge luxsier), unless otherwise specified.   *
*   ----------------------------------------------------------------------------------   *
*   Copyrights from authors other than Serge luxsier also apply here.                    *
*   Open source FREE licences also apply To the Code from the author (Serge luxsier)     *
*   ----------------------------------------------------------------------------------   *
*   Usual GNU FREE GPL-1,2, MIT... or whatever -  apply to this project.                 *
*   ----------------------------------------------------------------------------------   *
*   NOTE : All code source that I am the only author, I reserve for myself, the rights to*
*   make it to restricted private license.                                               *
******************************************************************************************/




#pragma once

#include <tuxvision/utility/sys/sys.h>
#include <string_view>

namespace tux
{
/**
 * \class strscan
 * \brief A utility class for scanning and analyzing string input with support for numeric parsing, literals, and identifiers.
 */
struct  strscan
{

    
    using iterator=std::string_view::iterator;
    tux::string::list words{};

    iterator     begin{};
    iterator     end{};
    iterator     cursor{};
    std::string_view  d{};


public:

    struct  [[maybe_unused]] context_data 
    {
        strscan::iterator begin{};
        strscan::iterator end{};
        strscan::iterator current{};
        //...
    };

    tux::string::location<std::string_view> location{};

    /**
     * \class numeric_data
     * \brief A class designed for handling and processing numeric data, providing functionalities to store, manipulate, and retrieve numerical information efficiently.
     */
    struct  [[maybe_unused]] numeric_data
    {
    private:
        static strscan _empty;
    public:
        std::string_view seq{};

        enum class base_enum:char 
        {
            binary,
            octal,
            decimal,
            hexadecimal,
            scexpr
        }base{strscan::numeric_data::base_enum::decimal};
        
        enum class size_type_enum:char 
        { i8, i16, i32, i64, u8, u16, u32, u64, f32, f64, f80, f128 } size_type{strscan::numeric_data::size_type_enum::i32};

        bool real{false};
        template<typename T> T value();

        [[nodiscard]] std::string  type_name() const;
        std::string  d{};
        context_data data{};


        [[nodiscard]] bool is_i8()          const{return size_type == numeric_data::size_type_enum::i8; }
        [[nodiscard]] bool is_i16()         const{return size_type == numeric_data::size_type_enum::i16; }
        [[nodiscard]] bool is_i32()         const{return size_type == numeric_data::size_type_enum::i32; }
        [[nodiscard]] bool is_i64()         const{return size_type == numeric_data::size_type_enum::i64; }
        [[nodiscard]] bool is_u8()          const{return size_type == numeric_data::size_type_enum::u8; }
        [[nodiscard]] bool is_u16()         const{return size_type == numeric_data::size_type_enum::u16; }
        [[nodiscard]] bool is_u32()         const{return size_type == numeric_data::size_type_enum::u32; }
        [[nodiscard]] bool is_u64()         const{return size_type == numeric_data::size_type_enum::u64; }
        [[nodiscard]] bool is_f32()         const{return size_type == numeric_data::size_type_enum::f32; }
        [[nodiscard]] bool is_f64()         const{return size_type == numeric_data::size_type_enum::f64; }
        [[nodiscard]] bool is_f80()         const{return size_type == numeric_data::size_type_enum::f80; }
        [[nodiscard]] bool is_f128()        const{return size_type == numeric_data::size_type_enum::f128; }
        [[nodiscard]] bool is_real()        const{return real; }
        [[nodiscard]] bool is_binary()      const{return base == numeric_data::base_enum::binary; }
        [[nodiscard]] bool is_octal()       const{return base == numeric_data::base_enum::octal; }
        [[nodiscard]] bool is_decimal()     const{return base == numeric_data::base_enum::decimal; }
        [[nodiscard]] bool is_hexadecimal() const{return base == numeric_data::base_enum::hexadecimal; }
        [[nodiscard]] bool is_scientific_expr() const{return base == numeric_data::base_enum::scexpr; }
        [[nodiscard]] bool is_integer()     const{return is_i8() || is_i16() || is_i32() || is_i64() || is_u8() || is_u16() || is_u32() || is_u64(); }

        double _value{.0l};
        void scale();

        strscan& scanner{strscan::numeric_data::_empty};

        strscan::iterator begin{};
        strscan::iterator end{};
        strscan::iterator cursor{};


        numeric_data()=default;
        explicit numeric_data(strscan& _scanner);
        ~numeric_data()=default;

        rem::code operator()(); ///< Numeric scan execution.

        rem::code scan_base2();
        rem::code scan_base8();
        rem::code scan_base10();
        rem::code scan_base16();


        using result = std::pair<rem::code, strscan::numeric_data>;
    }; //strscan::numeric_data.

    strscan()=default;
    explicit strscan(const std::string& str);
    explicit strscan(const char* str);
    explicit strscan(const std::string_view& str);

    strscan& operator = (const std::string& str);
    strscan& operator = (const char* str);
    strscan& operator = (const std::string_view& str);

    numeric_data::result                 scan_numeric();
    [[nodiscard]] std::pair<rem::code, std::string_view> scan_literal() const;
    [[nodiscard]] std::pair<rem::code, std::string_view> scan_identifier() const;
    std::pair<rem::code, std::string_view> scan(const std::function<rem::code(strscan&)>& scan_function);
    ~strscan()=default;

    bool skip_ws();
    // {
    //     while ((cursor < end ) && std::isspace(*cursor)) ++cursor;
    //     (void)sync();
    //     return (cursor < end);
    // }

    tux::string::location<std::string_view>& sync();
    rem::code seek(std::size_t offset);
    rem::code step(std::size_t offset);
    bool operator ++();
    bool operator ++(int);
    bool operator --();
    bool operator --(int);

    [[nodiscard]] tux::string mark() const;

    void               debug_dump(sys::file::handle _dlog=0) const;
    void               push();
    void               pop();
    [[nodiscard]] bool eof() const;
    bool has_text();


private:
    std::stack<strscan::iterator> stack{}; ///< saved cursor positions.

};

}

#include <tuxvision/utility/strscan.numeric_data.value.inc.h>
