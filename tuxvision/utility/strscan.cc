#include <tuxvision/utility/strscan.h>


namespace tux
{



strscan strscan::numeric_data::_empty{};

/**
 * Constructs a `strscan` object from a given string.
 * Initializes the internal string view and sets iterators for traversal.
 *
 * @param str The input string to be scanned.
 * @return None
 */
strscan::strscan(const std::string& str): d(str)
{
    begin   = d.begin();
    end     = d.end();
    cursor  = begin;
}


/**
 * Constructs a `strscan` object from a given C-style string.
 * Initializes the internal string view and sets iterators for traversal.
 *
 * @param str The input C-style string to be scanned.
 * @return None
 */
strscan::strscan(const char* str): d(str)
{
    begin   = d.begin();
    end     = d.end();
    cursor  = begin;
}


/**
 * Constructs a `strscan` object from a given string view.
 * Initializes the internal string view and sets iterators for traversal.
 *
 * @param str The input string view to be scanned.
 * @return None
 */
strscan::strscan(const std::string_view&str): d(str)
{
    begin   = d.begin();
    end     = d.end();
    cursor  = begin;
    location.begin = location.end = str.begin();
}


/**
 * Assigns a new string to the `strscan` object and updates its internal iterators.
 *
 * @param str The new string to be assigned to the `strscan` object.
 * @return A reference to the updated `strscan` object.
 */
strscan & strscan::operator=(const std::string &str)
{
    d = str;
    begin   = d.begin();
    end     = d.end();
    cursor  = begin;
    location.begin = location.end = str.c_str(); // MSVC: breaks here.
    return *this;
}


/**
 * Assigns a new character sequence to the `strscan` object.
 * Updates the internal string view and resets the iterators for traversal.
 *
 * @param str A pointer to the new character sequence to be assigned.
 * @return A reference to the updated `strscan` object.
 */
strscan & strscan::operator=(const char *str)
{
    d = str;
    begin   = d.begin();
    end     = d.end();
    cursor  = begin;
    location.begin = location.end = str;// MSVC: breaks here.
    return *this;
}


/**
 * Assigns a new string view to the `strscan` object.
 * Updates the internal data, iterators, and cursor position
 * based on the newly assigned string view.
 *
 * @param str The new string view to be assigned.
 * @return A reference to the updated `strscan` object.
 */
strscan & strscan::operator=(const std::string_view &str)
{
    d = str;
    begin   = d.begin();
    end     = d.end();
    cursor  = begin;
    location.begin = location.end = str.begin();

    return *this;
}


/**
 * Scans the input for numeric data at the current position of the scanner.
 * Attempts to parse numeric values using the `numeric_data` object and evaluates
 * the result of the scan for validity.
 *
 * @return A `strscan::result` object containing:
 *         - `rem::code::accepted` and the associated `numeric_data` if numeric data was effectively scanned.
 *         - `rem::code::rejected` and an empty `numeric_data` if the sequence was not a numerical expression.
 */
strscan::numeric_data::result strscan::scan_numeric()
{
    if (strscan::numeric_data data(*this); !!data())
    {
        sync();
        return {rem::code::accepted, data};
    }
    return {rem::code::rejected, {}};
}


/**
 * Scans for a literal string enclosed in single or double quotes at the current cursor position.
 * Detects whether the input starts with a valid quote character and attempts to locate the closing
 * quote, handling escaped characters if present.
 *
 * @return A `std::pair` containing:
 *         - `rem::code::accepted` and a valid `std::string_view` when a complete quoted literal is found.
 *         - `rem::code::rejected` and an empty `std::string_view` if the literal is incomplete or not detected.
 *
 * @note Cannot scan for encoded escaped sequences such as ansi's CSI sequences.
 */
std::pair<rem::code, std::string_view> strscan::scan_literal() const
{

    auto cc = cursor;
    if ((*cc != '"') && (*cc != '\'')) return {rem::code::rejected, {}};
    auto q = *cc; // Save the quote char.
    cc++;
    do
    {
        if (*cc == '\\') { ++cc; ++cc; }
        if (*cc == q)
            return {rem::code::accepted, std::string_view(cursor, cc-cursor+1)};
        ++cc;
    }while (cc != end);
    auto l = sys::error(1); l << "unterminated literal." << l;
    return {rem::code::rejected, {}};
}


/**
 * Scans the input string for a valid identifier starting at the current cursor position.
 * An identifier is defined as a sequence beginning with an alphabetic character and
 * followed by alphanumeric characters.
 *
 * @return A pair where the first element is a `rem::code` status indicating whether the
 *         scan was accepted or rejected, and the second element is a `std::string_view`
 *         representing the identified string if accepted, or an empty view otherwise.
 */
std::pair<rem::code, std::string_view> strscan::scan_identifier() const
{

    auto cc = cursor;
    if (!isalpha(*cc)) return {rem::code::rejected, {}};
    do
    {
        if ((!isalnum(*cc)) && (*cc != '_')) return {rem::code::accepted, std::string_view(cursor, cc-cursor)};
        ++cc;
    }while (cc <= end);
    return {rem::code::accepted, std::string_view(cursor, cc-cursor)};
}


/**
 * Scans the string using the provided scanning function.
 * Executes the supplied scanning function to determine the result
 * and returns an appropriate status code along with a processed
 * substring view. Logs an error if the scanning function is null.
 *
 * @param scan_function A callable scanning function that operates on the `strscan` object.
 *                       The function should accept a reference to the `strscan` instance
 *                       and return a `rem::code` result indicating the scan status.
 * @return A `std::pair` where the first element is a `rem::code` status code
 *         indicating the result of the scan, and the second element is
 *         a `std::string_view` that represents the processed substring.
 */
std::pair<rem::code, std::string_view> strscan::scan(const std::function<rem::code(strscan &)>& scan_function)
{
    const auto cc = cursor;
    if (scan_function && !!scan_function(*this))
        return {rem::code::accepted, std::string_view(cc, cursor-cc)};

    auto log = sys::error(); log << "scanner function cannot be null." << log;
    return {rem::code::null_ptr, {}};
}


/**
 * Skips over any whitespace characters in the scanned string.
 * Updates the cursor position and tracks changes in line and column number.
 *
 * @return True if there are characters remaining in the string after skipping whitespace, otherwise false.
 */
bool strscan::skip_ws()
{
    auto cc = cursor;
    while ((cc < end) && isspace(*cc))
    {
        switch (*cc)
        {
            case '\n':
                ++cc;
                if (*cc == '\r') ++cc;
                location.line++;
                location.column = 1;
                break;
            case '\r':
                ++cc;
                if (*cc == '\n') ++cc;
                location.line++;
                location.column = 1;
                break;
            default:
                ++cc;
                location.column++;
                break;
        }
    }
    cursor = cc;
    location.offset = cursor - begin;
    return cursor < end;
}

/**
 * Synchronizes the current state of the scanner's location object with the position of the cursor.
 * Updates the line, column, and offset values based on the position of the cursor within the scanned range.
 *
 * @return A reference to the updated location object.
 */
tux::string::location<std::string_view> & strscan::sync()
{
    auto cc = begin;
    location.line = location.column = 1;
    while ((cc != end) && (cc < cursor))
    {
        switch (*cc)
        {
            case '\n':
                ++cc;
                if (*cc == '\r') ++cc;
                location.line++;
                location.column = 1;
                break;
            case '\r':
                ++cc;
                if (*cc == '\n') ++cc;
                location.line++;
                location.column = 1;
                break;
            // case '\t':
            //     location.column += 4; // Go figure out how many spaces this 'TAB CHARACTER' is from here! ...
            //     ++cc;
            //     break;
            default:
                ++cc;
                location.column++;
                break;
        }
    }
    location.offset = cursor - begin;
    return location;
}


/**
 * Seeks the cursor of the `strscan` object to a new position relative to its current position.
 * Adjusts the internal cursor by the provided offset if it is within the valid range.
 * Syncs the internal status after moving the cursor.
 *
 * @param offset The number of characters to move the cursor forward.
 * @return `rem::code::done` if the seek operation is successful.
 *         `rem::code::rejected` if the requested offset exceeds the valid range.
 */
rem::code strscan::seek(std::size_t offset)
{
    if (offset > end - cursor) return rem::code::rejected;
    cursor += offset;
    (void)sync();

    return rem::code::done;
}


/**
 * Advances the internal cursor by a specified offset.
 * Ensures the cursor does not surpass the end of the string and synchronizes state afterwards.
 *
 * @param offset The number of positions to move the cursor forward.
 * @return A status code indicating the operation's completion.
 */
rem::code strscan::step(std::size_t offset)
{
    cursor += offset;
    if (cursor > end) cursor = end;
    (void)sync();
    return rem::code::done;
}


/**
 * Advances the `strscan`'s cursor to the next position in the string.
 * Ensures synchronization after moving the cursor.
 *
 * @return True if the cursor was successfully advanced, false if it reached the end.
 */
bool strscan::operator++()
{
    if (cursor == end) return false;
    cursor++;
    //(void)sync();
    return true;
}


bool strscan::operator++(int)
{
    if (cursor == end) return false;
    cursor++;
    //(void)sync();
    return true;
}


bool strscan::operator--()
{
    if (cursor == begin) return false;
    cursor--;
    //(void)sync();
    return true;
}


bool strscan::operator--(int)
{
    if (cursor == begin) return false;
    cursor--;
    //(void)sync();
    return true;

}


/**
 * Marks the current cursor position within the scanned string.
 * Constructs a visual representation of the current line containing the cursor,
 * along with an indicator pointing to the exact position of the cursor.
 *
 * @return A `tux::string` object representing the marked line with a cursor pointer.
 */
tux::string strscan::mark() const
{
    std::string_view line;
    tux::string      text;
    auto             cc = cursor;
    while ((cc > begin) && (*cc != '\n') && (*cc != '\r')) --cc;
    if (*cc =='\n' || *cc =='\r') cc++;
    auto line_start = cc;
    cc = cursor;
    while ((cc < end) && (*cc != '\n') && (*cc != '\r')) ++cc;
    line = {line_start, static_cast<size_t>(cc-line_start)};
    int len = static_cast<int>(cursor-line_start);
    if (len < 0) return "";
    text << line << '\n';
    if (len>0)
         text << tux::string::fill(' ', len);
    text << glyph::c_left_towards_up_arrow;
    return text();
}


/**
 * Outputs debug information to a leaf log file.
 * Provides detailed information about the current state of the `strscan`
 * object, including its line, column, and offset, and writes it to the
 * specified leaf log handle.
 *
 * @param _dlog The leaf log file handle where debug information
 *              will be written.
 * @return None
 */
void strscan::debug_dump(sys::file::handle _dlog) const
{
    auto log = sys::debug(_dlog);
    log << "--------------------------------------" <<log;
    log << "  line  : " << location.line << log;
    log << "  column: " << location.column << log;
    log << "  offset: " << location.offset << log;
    log << "--------------------------------------" << log << mark() << log;
}


void strscan::push()
{
    stack.push(cursor);
}


void strscan::pop()
{
    if (!stack.empty())
        cursor = stack.top();
    stack.pop();
}


bool strscan::eof() const
{
    return cursor >= end;
}


bool strscan::has_text()
{
    return !d.empty();
}


/////////////------------------------------------ Numeric scanners ----------------------------------------------------
#pragma region numeric_scanners


std::string strscan::numeric_data::type_name() const
{
    std::map<numeric_data::size_type_enum, std::string> map ={
        {size_type_enum::i8, "i8"},
        {size_type_enum::u8, "u8"},
        {size_type_enum::i16, "i16"},
        {size_type_enum::u16, "u16"},
        {size_type_enum::i32, "i32"},
        {size_type_enum::u32, "u32"},
        {size_type_enum::i64, "i64"},
        {size_type_enum::u64, "u64"},
        {size_type_enum::f32, "f32"},
        {size_type_enum::f64, "f64"}
    };
    return map[size_type];
}


/**
 * Determines and assigns the appropriate size type for a given numeric value.
 * This method evaluates the range of `_value` and sets the `size_type`
 * member variable accordingly, representing the numeric type as one of the
 * enumerations in `size_type_enum`.
 *
 * The ranges assessed include signed and unsigned 8-bit, 16-bit, 32-bit,
 * and 64-bit values based on the `_value`. If `_value` is less than 0.0,
 * it is treated as signed; otherwise, it is treated as unsigned.
 *
 * @return None
 */
void strscan::numeric_data::scale()
{
    // Signed Integer:
    // -------------------------------------------------------------------------------
    // 8bits:
    if((_value >=-128) && (_value <=127))
    {
        size_type = size_type_enum::i8;
        return;
    }
    if((_value >= 128) && (_value <=255))
    {
        size_type = size_type_enum::u8;
        return;
    }
    if((_value >= -32768) && (_value <= 32767))
    {
        size_type = size_type_enum::i16;
        return;
    }
    if((_value >=32768) && (_value <= 65565))
    {
        size_type = size_type_enum::i16;
        return;
    }
    if((_value >= -2147483648) && (_value <= 2147483647))
    {
        size_type = size_type_enum::i32;
        return;
    }
    if((_value >= 0x10000) && (_value <= 4294967295))
    {
        size_type = size_type_enum::u32;
        return;
    }

    if(_value < 0.0)
        size_type = size_type_enum::i64;
    else
        size_type = size_type_enum::u64;
}


/**
 * Constructs a `numeric_data` object from a given `strscan` instance.
 * Initializes the iterators for numeric scanning to match the current cursor of the given scanner.
 *
 * @param _scanner The `strscan` instance to associate with the numeric data.
 * @return None
 */
strscan::numeric_data::numeric_data(strscan&_scanner):scanner(_scanner)
{
    begin = cursor = scanner.cursor;
    end = scanner.end;
}






/**
 * Executes the numeric scan operation associated with the `numeric_data` object.
 *
 * @return A `rem::code` enumeration value indicating the status of the numeric scan.
 */
rem::code strscan::numeric_data::operator()()
{
    if (cursor == end) return rem::code::empty;
    if (!scan_base2() && !scan_base8() && !scan_base10() && !scan_base16()) return rem::code::rejected;

    return rem::code::accepted;
}


/**
 * Parses a binary (base-2) numeric sequence from the current cursor position.
 * Reads sequences such as "0b1101" or similar binary representations, validates
 * the format, and computes the corresponding numerical value. Handles separators ('\'')
 * within the sequence but rejects invalid formats or characters.
 *
 * @return rem::code::accepted if the binary sequence is successfully parsed and validated.
 *         rem::code::rejected if the input does not conform to the expected format or encounters invalid characters.
 */
rem::code strscan::numeric_data::scan_base2()
{
// 0b11010101010101010101010101010101010                            --> Parse all
    // 0b1101'0101'01010101'''''''0101'0101'0101'0101'0100'1010     --> Parse all
    // 0b1101 0101 0101 0101 0101 0101 0101 0101 0100'1010          --> Parse : 0b1101
    // 11010101'01010101'01010101'01010101'01001010B                --> Parse : Rejected ( not handling suffix )
    // 11010101 01010101 01010101 01010101 01001010                 --> Parse :  [base 10] == 11'010'101


    auto a = cursor;
    tux::string buf;
    base = base_enum::binary;

    if(std::toupper(*a) == 'B')
        buf << *a++;
    else
    {
        if(*a == '0')
        {
            buf << *a++;
            if (std::toupper(*a) == 'B')
                buf << *a++;
            else
                return rem::code::rejected;

        }
    }
    if(a == cursor)
        return rem::code::rejected;


    loop2:
    if(*a == '\'') ++a;
    if(!isdigit(*a))
        return rem::code::rejected;

    while((a != end) && std::isdigit(*a)){
        if(*a>='2')
            return rem::code::rejected;

        buf << *a++;
    }
    if((a != end) && *a =='\'')
    {
        ++a;
        goto loop2;
    }

    if((a==cursor) || (buf.empty())) return rem::code::rejected;// duh!!!

    seq = {cursor, a};
    _value = std::stoi(buf(), nullptr, 2);
    base = base_enum::binary;
    scale();
    return rem::code::accepted;
}


/**
 * Scans the current string for an octal (base-8) numeric value.
 * It checks for valid octal prefixes and processes digits within the range 0-7.
 * Supports apostrophes as separators within the number. If the input is invalid,
 * the scan is rejected.
 *
 * @return A value of type `rem::code`, indicating whether the scan was accepted or rejected.
 */
rem::code strscan::numeric_data::scan_base8()
{
    //rem::debug() << rem::fn::func;
    auto a = cursor;
    tux::string buf;
    base = strscan::numeric_data::base_enum::octal;
    if(std::string_view prefixes = "ooq@&"; prefixes.find(*a) != std::string_view::npos )
        a++;
    else
    {
        //rem::write() << " base 8 :rejected @prefix '" << color::yellow << *a << color::reset << "'.";
        return rem::code::rejected;
    }

    loop8:
    //Rem::Debug() << "loop8: A on '" << Color::Yellow << *A << Color::Reset << '\'';
    while((a != end) && std::isdigit(*a) && (*a <= '7'))
    {
        buf << *a++;
        //rem::debug() << " pushed digit'" <<  color::yellow << *(a-1) << color::reset << "' into the buffer. next:'" << color::yellow << *a << color::reset << "'";
    }
    if((a != end) && (*a == '\''))
    {
        ++a;
        goto loop8;
    }
    //rem::debug() << "octal loop exit: a on '" << color::yellow << *a << color::reset << '\'';

    if(((std::isdigit(*a) && (*a >= '7'))) || (a==cursor))
        return rem::code::rejected;

    //--a;
    //rem::debug() << "base8: a on '" << color::yellow << *a << color::reset << "' - buffer: [" << color::yellow << buf << color::reset << "]";
    if(buf.empty()) return rem::code::rejected;
    seq = {begin, a};
    _value = std::stoi(buf(), nullptr, 8);
    base = strscan::numeric_data::base_enum::octal;
    scale();
    return rem::code::accepted;
}


/**
 * Scans and processes a base-10 numeric sequence from the current cursor position.
 * This method identifies and validates decimal or real number formats while handling
 * special cases such as separators or multiple decimal points. The processed value
 * is stored and categorized based on its type (integer or floating-point).
 *
 * @return `rem::code::accepted` if the sequence is successfully processed and valid,
 *         `rem::code::rejected` if the sequence fails validation.
 */
rem::code strscan::numeric_data::scan_base10()
{

    auto a = cursor;
    base = strscan::numeric_data::base_enum::decimal;

    tux::string buf;
    //if((*a == '.') || (*a == ','))
    if(*a == '.')
    {
        real = true;
        buf << '.'; // force '.' for convertion using tux::string >> ;
        ++a;
    }

    if(*a == '\'') ++a;

    if(!std::isdigit(*a) ){
        //auto l = sys::status(); l << " base 10 :rejected @prefix '" << color::yellow << *a << color::reset << "'." << l;
        return rem::code::rejected;
    }


    while((a != end) && std::isdigit(*a))
    {
        //rem::debug() << "while loop: a on '" << color::yellow << *a << color::reset << '\'';
        buf << *a++;
        //rem::debug() << "next to digit: a on '" << color::yellow << *a << color::reset << '\'';
        if(*a == '\''){++a; continue; }
        if((*a == '.'))// || (*a == ','))
        {
            if(real)
            {
                //rem::status() << " rejected on '" << color::yellow << *a << color::reset << "' - real flag already set.";
                return rem::code::rejected;
            }
            real = true;
            buf << '.';
            ++a;
            //continue;
        }
        //rem::debug() << "bottom while loop: a on '" << color::yellow << *a << color::reset << '\'';
    }
    //rem::debug() << "a on '" << color::yellow << *a << color::reset << '\'';
    if(a == cursor)
        return rem::code::rejected;


    seq = {cursor, a};
    base = strscan::numeric_data::base_enum::decimal;

    buf >> _value;
    if(!real)
        scale();
    else
        size_type = size_type_enum::f32;

    // -- Sous réserve :
    scanner.location.offset = a - scanner.begin;
    scanner.location.begin = cursor;
    scanner.location.end = a;
    scanner.location.length = a - cursor + 1; // Or seq.length();
    //---------------------------------------
    return rem::code::accepted;
}


/**
 * Scans a sequence of characters starting from the cursor and interprets it as a hexadecimal number.
 * Updates internal state variables, including the base, sequence range, and parsed value, upon successful detection.
 * Rejects the input if it does not conform to valid hexadecimal format.
 *
 * @return `rem::code::accepted` if the scan successfully interprets a hexadecimal number, otherwise `rem::code::rejected`.
 */
rem::code strscan::numeric_data::scan_base16()
{
    //rem::debug() << " base 2,8 rejected - then:";
    auto a = cursor;
    tux::string buf;
    base = strscan::numeric_data::base_enum::hexadecimal;
    // do not want to add '$'
    if (*a == '0')
    {
        ++a;

        if(std::toupper(*a) != 'X')
        {
            //rem::write() << " base 16 :rejected @prefix '" << color::yellow << *a << color::reset << "'.";
            return rem::code::rejected;
        }
    }
    else
        if(*a !='$') // hard coded
            return rem::code::rejected;
    ++a;

    loop16:
    //rem::debug() << "loop16: a on '" << color::yellow << *a << color::reset << '\'';
    while((a != end) && std::isxdigit(*a)) buf << *a++;
    if((a != end) && (*a == '\'')){ ++a; goto loop16; }
    if(a==cursor) return rem::code::rejected;
    //rem::debug() << rem::fn::func << "buffer contents '" << color::yellow << buf() << color::reset << '\'';
    seq = {cursor, a};
    //    Rem::Debug() << "[Hex] : Sequence ["
    //    << Color::Yellow << std::string(num_details.seq.begin(), num_details.seq.end())
    //    << Color::Reset << "] Length: "
    //    << Color::LightCoral << num_details.seq.length()
    //    << Color::White <<  " -> Buffer:["
    //    << Color::Yellow << Buf
    //    << Color::Reset << "] ";
    if(buf.empty()) return rem::code::rejected;
    _value = std::stoi(buf(), nullptr, 16);
    scale();
    return rem::code::accepted;
}




#pragma endregion numeric_scanners
/////////////------------------------------------ Numeric scanners END-------------------------------------------------
}
