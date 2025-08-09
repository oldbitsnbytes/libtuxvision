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
#include <tuxvision/utility/sys/sys.h>
#include <tuxvision/lexer/token_info.h>

#include <tuxvision/utility/cmdargs.h>
#include <tuxvision/utility/notify.h>
#include <tuxvision/utility/strscan.h>


namespace tux::lang
{

class lexer
{
    //std::string_view text{};
    strscan scanner{};
public:
    struct config_data
    {
        token_reference_table*  tokens_table{nullptr};
        token_info::list*       tokens{nullptr};
        std::string_view        text{};
    }source;

    lexer()=default;
    lexer(token_reference_table* table, std::string_view a_text);
    ~lexer()=default;

    rem::code operator >> (token_info& token); ///< Experimenting with this coding style...

    lexer::config_data& config() { return source; }
    lexer& operator=(lexer::config_data& data);
    std::pair<rem::code, size_t> parse();

    void debug_dump() const;
private:
    bool F4AC = false;
    rem::code parse_token(token_info& token);
    rem::code parse_identifier(token_info& token);
    rem::code parse_numeric(token_info& token);
    rem::code parse_quotes(token_info& token);
    rem::code parse_operator(token_info& token);
    //rem::code parse_keyword(token_info& token);
    rem::code parse_comment(token_info& token);
    rem::code parse_line_comment(token_info& token);
    rem::code parse_block_comment(token_info& token);
    rem::code parse_f4ac(token_info& token);
    rem::code push(token_info& token);
    //...
};

} // namespace tux::lang
