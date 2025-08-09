//
// Created by oldlonecoder on 25-05-24.
//

//#ifndef TOKEN_INFO_H
//#define TOKEN_INFO_H
////////////////////////////////////////////////////////////////////////////////////////////
//   Copyright (C) ...,2025,... by Serge Lussier
//   serge.lussier@oldbitsnbytes.club / lussier.serge@gmail.com
//   ----------------------------------------------------------------------------------
//   Unless otherwise specified, all Codes and files in this project is written
//   by the author and owned by the author (Serge Lussier), unless otherwise specified.
//   ----------------------------------------------------------------------------------
//   Copyrights from authors other than Serge Lussier also apply here.
//   Open source FREE licences also apply to the code from the author (Serge Lussier)
//   ----------------------------------------------------------------------------------
//   Usual GNU FREE GPL-1,2, MIT... or whatever -  apply to this project.
//   ----------------------------------------------------------------------------------
//   NOTE : All source code that I am the only author, I reserve for myself, the rights to
//   make it to restricted private license.
////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------



#pragma once
#include <tuxvision/lexer/components.h>
#include <tuxvision/utility/object.h>
#include <tuxvision/utility/strscan.h>

namespace tux::lang
{

struct token_info
{
    lang::type::value       t{lang::type::Any};
    lang::type::value       s{lang::type::Null};
    lang::mnemonic          m{lang::mnemonic::Null};
    std::string_view        name{"mnemonic::Undefined"}; // Pas vraiment le choix d'ajouter ça ici...
    lang::operand::Index    d = lang::operand::Identifier;
    tux::string::location<> cursor{};

    struct propbits
    {
        uint8_t V: 1; // This token is a usable value into expression.
        uint8_t S: 1; // Post semantic parser: Left-Value Assignable Object
        uint8_t M: 1; // Virtual multiply operator. ( ex.: "4ac" => 4*a*c )
        uint8_t U: 1; // This token is a single utf-8 symbol { ex.:π,➪, ... }
    }properties{0,0,0,0};



    explicit           operator bool() const;
    bool               operator == (const token_info& other) const;
    bool               operator != (const token_info& other) const;
    [[nodiscard]] bool is_numeric_literal() const;
    [[nodiscard]] bool is_comment() const { return m==lang::mnemonic::LineComment || m==lang::mnemonic::BeginComment;}

    using list = std::vector<token_info>;
    [[nodiscard]] std::string to_string() const;
    [[nodiscard]] std::string_view text() const;
    [[nodiscard]] std::string mark() const;

    [[nodiscard]] bool is_operator() const { return static_cast<bool>(s & lang::type::Operator); }
    [[nodiscard]] bool is_identifier() const { return static_cast<bool>(s & lang::type::Id); }
    [[nodiscard]] bool is_keyword() const { return static_cast<bool>(s & lang::type::Keyword); }
    [[nodiscard]] bool is_literal() const { return static_cast<bool>(s & lang::type::Literal); }
    [[nodiscard]] bool is_quote() const { return m==lang::mnemonic::SQuote || m==lang::mnemonic::DQuote; }
    [[nodiscard]] bool is_punctuation() const { return static_cast<bool>(s & lang::type::Punctuation); }

};



class token_reference_table : public object
{
protected:
    token_info::list reference{};
public:
    token_reference_table() = default;
    ~token_reference_table() override;

    virtual rem::code set();
    virtual token_info scan(strscan& str_scanner);
    virtual token_info get_by_mnemonic(tux::lang::mnemonic m);
};


} // namespace end  tux::lang
