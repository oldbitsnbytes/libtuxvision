#include "token_info.h"


namespace tux::lang
{
/**
 * Evaluates the validity of the current token_info object by checking if the cursor's
 * beginning position is non-null. This determines whether the token information is
 * considered valid or invalid.
 *
 * @return true if the cursor's begin position is not null, indicating the token_info object
 *         is valid. Returns false otherwise.
 */
token_info::operator bool() const
{
    return cursor.begin != nullptr;
}


/**
 * Compares the current token_info object with another token_info object to determine
 * if they are equal. This equality check is based on the mnemonic property of the objects.
 *
 * @param rhs A reference to another token_info object to compare against.
 * @return true if the mnemonic property of the current object is equal to that of rhs,
 *         false otherwise.
 */
bool token_info::operator==(const token_info& rhs) const
{
    return rhs.m == m;
}


/**
 * Compares the current token_info object with another token_info object to determine
 * if they are not equal. This comparison is based on the mnemonic property of the objects.
 *
 * @param rhs A reference to another token_info object to compare against.
 * @return true if the mnemonic property of the current object is not equal to that of rhs,
 *         false otherwise.
 */
bool token_info::operator!=(const token_info& rhs) const
{
    return rhs.m != m;
}


/**
 * Determines whether the current token_info object represents a numeric literal.
 * This is verified by checking if the mnemonic of the token is equivalent to
 * lang::mnemonic::Numeric.
 *
 * @return true if the token's mnemonic is lang::mnemonic::Numeric, indicating
 *         that the token represents a numeric literal. Returns false otherwise.
 */
bool token_info::is_numeric_literal() const
{
    return m==lang::mnemonic::Numeric;
}


/**
 * Converts the current token_info object into a string representation, providing
 * detailed information such as the token's reference name, type(s), text, cursor position,
 * and any additional markings. This representation is formatted using predefined colors for
 * clarity and debugging purposes.
 *
 * @return A string containing the formatted representation of the token_info object,
 *         including its name, type(s), text, cursor details, and markings.
 *
 * @note As of [June 01, 2025], this function does not produce the intended output format yet.
 *       It is a placeholder for future development.
 *       The function is currently a rough raw text containing new line and raw attributes.
 *       It is not yet functional inside the projected env. - i.e.: Will Creates a leafpad object describing the  token_info's informations in a vchar::bloc output format.
 *       It is not yet ready for production.
 */
std::string token_info::to_string() const
{
    tux::string l;
    tux::string mname;
    mname << "[" << color::cadetblue2 << name << color::r << "]";
    l << color::white << std::format("({:>3d},{:<3d}) ", cursor.line,cursor.column) <<color::r << std::format("{:<30}", mname());
    l << color::r << " \"" << color::yellow  << text() << color::r << "\" [" << color::lime << lang::component::type_name(t) <<
        color::r << "] {" << color::lightcyan3 << lang::component::type_name(s) << color::r << "}";

    // l << *cursor << "\n";
    // l << mark() << "\n";
    return l();
}


std::string_view token_info::text() const
{
    return {cursor.begin, cursor.length};
}


std::string token_info::mark() const
{
    tux::string      text;

    ////// Take the address of the beginning of the source text by subtracting the offset from the cursor's begin address.
    ///Preventing out of range access.
    auto ptr= cursor.begin-cursor.offset;

    auto cc = cursor.begin;
    while ((cc > ptr) && (*cc != '\n') && (*cc != '\r')) --cc;
    if (*cc =='\n' || *cc =='\r') cc++;
    auto line_start = cc;
    cc = cursor.begin;
    while (cc && (*cc != '\n') && (*cc != '\r')) ++cc;
    //if (*cc =='\n' || *cc =='\r') --cc;
    const std::string_view line = {line_start, static_cast<size_t>(cc - line_start)};
    text << line << '\n' << tux::string::fill(' ', cursor.begin  - line_start);
    text << glyph::c_left_towards_up_arrow;

    return text();


}


token_reference_table::~token_reference_table()
{
    reference.clear();
}


/**
 * Sets the token reference table. If the reference table is already populated, no changes
 * are made and a corresponding status code is returned. If the table is empty, it is rejected and nothing is done.
 *
 * @return A code value indicating the status of the operation:
 *         - rem::code::exist: The reference table is already populated.
 *         - rem::code::done: The operation completed successfully and the table is set.
 */
rem::code token_reference_table::set()
{
    if (!reference.empty())
        return rem::code::exist;
    //...
    reference = {
        #include <tuxvision/lexer/default_tokens_reference.hcc>
    };
    return rem::code::done;
}




/**
 * Scans the provided string scanner for a token that matches an entry in the token
 * reference table. Iterates over the token reference table to find a match for the
 * current substring pointed to by the scanner's cursor. Updates the cursor position
 * of the scanner upon a successful match. Returns an empty token_info object if no
 * match is found or if the input is invalid.
 *
 * @param str_scanner A reference to the strscan object, which provides the string
 *                    input to be scanned. The cursor of this object is used to
 *                    determine the current scan position.
 * @return A token_info object representing the matched token, including metadata
 *         such as its location and length. If no match is found, returns an empty
 *         token_info object.
 */
token_info token_reference_table::scan(strscan&str_scanner)
{
    auto src = str_scanner.cursor;
    if (!src){
        auto l = sys::error(1); l << "no source." << l;
        return {};
    }

    if(reference.empty())
    {
        auto l = sys::warning(1); l << " tokens reference table is empty" << l;
        return {};
    }

    int unicode = 0;
    if (!*src)
    {
        auto l = sys::info(); l << rem::code::eof << l;
        return {};
    }

    for (auto token : reference)
    {
        auto crs = src;
        auto ref = token.cursor.begin;
        //l << "token ref : '" << color::yellow << token.cursor.begin << color::r << "'\n";
        unicode = 0; // oops...
        //std::size_t sz = std::strlen(rtxt);

        if(*crs != *ref) {
            //l << "next ref" <<l;
            continue;
        }
        while ((ref && crs) && (*crs && *ref) && (*crs == *ref))
        {
            if (*crs < 0)
                ++unicode; ///< @note oui/ yes; Soon/Bientôt je supporte quelques symboles UTF-8 (pi, xor,...)
            //l << color::yellow << *crs << color::r << '|' << color::hotpink4 << ref << color::r << l;
            ++crs;++ref;
            //l << color::yellow << *crs << " <=> " << color::hotpink4 << ref << color::r << l;
        }
        if (*ref == 0)
        {

            if (*crs && !isspace(*crs) && !token.is_operator())
                if ((isalnum(*crs) || (*crs == '_')) && !(token.t & (lang::type::Operator|lang::type::Punctuation|lang::type::Keyword))) continue;


            token.cursor.begin = src;
            token.cursor.end = --crs;
            token.cursor.length = (token.cursor.end - token.cursor.begin)+1;
            //l = sys::info(1); l << "matched : " << lang::component::type_name(token.t) << l;
            token.cursor.offset = token.cursor.begin - str_scanner.begin;
            token.cursor.line = str_scanner.location.line;
            token.cursor.column = str_scanner.location.column;
            //str_scanner.cursor = crs;   // Now we update the cursor position.

            return token;
        }
    }

    //l = sys::debug(1); l << " No match." << l;
    return {};
}


token_info token_reference_table::get_by_mnemonic(tux::lang::mnemonic m)
{
    for (auto token : reference)
        if (token.m == m)
            return token;
    return {};
}
} // end namespace tux::lang
