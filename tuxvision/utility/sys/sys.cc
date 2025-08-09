#include <tuxvision/utility/sys/sys.h>
#include <tuxvision/utility/vchar.h>

using namespace tux;
namespace fs = std::filesystem;

//////////////////////////////////////////////
/// \brief sys::files
/// \note The static-global sys files array systematically init default 0'th index (STDOUT_FILE[fd #1]) with std::cout.
sys::file::list sys::files
{
    {"std::out", &std::cout}
};
sys::file::handle sys::_default_file_handle=0; ///< std::cout by default.
//////////////////////////////////////////////



////////////////////////////////////////////////
/// \brief sys::file::close
///        closes the opened file by this file struct is the descriptor is not stdout (fd:#1)
/// \return accepted or rejected
///
rem::code sys::file::close()
{
    if(fileptr){
        if(fileptr != &std::cout){
            delete fileptr;
            fileptr = nullptr;
            return rem::code::accepted;
        }
    }
    return rem::code::rejected;
}

////////////////////////////////////////////////////
/// \brief sys::out constructor
/// \param out_ can be nullptr if it is required to use the default std::cout stream.
/// \param message sys output message type
/// \param src implicit / automatic source location.
///
sys::out::out(std::ostream* out_, rem::type message, std::source_location&& src):
    ofs(out_),
    type(message),
    code(rem::code::ok),
    location(src){
    if (type != rem::type::output)
        init_header();
}


////////////////////////////////////////////////////
/// \brief sys::out constructor specific for the header components
/// \param out_ can be nullptr if it is required to use the default std::cout stream.
/// \param message sys output message type
/// \param src implicit / automatic source location.
///
sys::out::out(std::ostream *file_ptr, rem::type message, header_component hc, std::source_location &&src):
    ofs(file_ptr),
    type(message),
    _headercomp_(hc),
    code(rem::code::ok),
    location(src)
{
    if(!hc.type) {
        hc.type = 1; hc.col  = 1; hc.frame = 1; hc.file = 1; hc.stamp = 1; hc.fun = 0;
        _headercomp_ = hc;
    }

    if (type != rem::type::output)
        init_header();
}

sys::out::~out()
{
    header.clear();
    text.clear();
}

#pragma region leaf_out

//////////////////////////////////////////////////////////////////////////////
/// \brief sys::out::init_header
///        internal call from the constructor.
///
void sys::out::init_header()
{

    tux::string dash;
    tux::string hline;
    hline << color::r;
    if(_headercomp_.frame){
        if(_headercomp_.hline){
            for(int x=0; x < sys::files[0].page_width ; x++)
            {
                hline << tux::cadre()[tux::cadre::Horizontal];
                dash << "-";
            }
            (*ofs) << hline() << std::endl;
        }
    }

    if(_headercomp_.stamp){
        auto txt{tux::string::now("%H:%M:%S")};
        auto [ic, a] = rem::function_attributes(rem::fn::stamp);
        tux::string acc;
        acc << a.fg << glyph::data[ic]  << txt;
        header << acc() << color::r << " ";

    }
    if(_headercomp_.type){
        auto [gh,colors] = rem::type_attributes(type);
        header << colors << gh  << ' ' << rem::to_string(type) << color::reset << ' ';
    }

    if(_headercomp_.file){
        auto [gh, colors] = rem::function_attributes(rem::fn::file);
        tux::string txt = location.file_name();
        tux::string::word::list words;
        if(auto count = txt.words(words, false, "/"); !count) return ;
        header << color::reset << " in " << colors << gh;
        if(words.size() > 1) {
            auto i = words.end();
            --i; // filename
            auto filename = *i;
            --i; // parent dir
            header << **i;
            ++i;
            header << '/' << **i << color::r << ' ';
        }
        words.clear();
    }
    // disable uncontrollable and potentially insane length of function signature given by std::source_location.
    // instead, using it explicitly into a distinct book input
    //------------------------------------------------------------------------------------------------------------
    //    if(headercomp.fun)
    //    {
    //        auto [gh, colors] = sys::functionattributes(rem::fn::fun);
    //        header << colors << gh << ' ' << location.function_name() << color::reset;
    //    }
    //------------------------------------------------------------------------------------------------------------
    if(_headercomp_.line){
        auto [gh, colors] = rem::function_attributes(rem::fn::line);
        //header << colors << "line: " << std::format("{}", location.line()) << color::reset << ' ';
        header << colors << "line: " << location.line() << color::r << ' ';
        (*ofs) << header();
    }


    header = "";
    (*ofs) << dash() << std::endl;
    if(_headercomp_.fun && type != rem::type::test){
        auto [gh, colors] = rem::function_attributes(rem::fn::func);
        header << colors << gh << ' ' << location.function_name()<< color::r;

        (*ofs) << header();// << std::endl;
        header = "";
    }
    (*ofs) << dash() << std::endl;
}

/////////////////////////////////////////////////////////////////////////////
/// \brief sys::out::operator <<
/// \param txt
/// \return
///
sys::out &sys::out::operator <<(const std::string &txt)
{
    (*ofs) << txt;
    return *this;
}



sys::out& sys::out::operator << (glyph::type f)
{
    (*ofs) << glyph::data[f];
    return *this;
}

/////////////////////////////////////////////////////////////////////////
/// \brief sys::out::operator <<
/// \param cstr
/// \return
///
sys::out& sys::out::operator << (const char* cstr)
{
    ofs->write(cstr,std::strlen(cstr));
    return *this;
}


////////////////////////////////////////////////////////////////////
/// \brief sys::out::operator <<
/// \param cstr
/// \return
///
sys::out& sys::out::operator << (std::string_view cstr)
{
    ofs->write(cstr.data(),cstr.length());
    return *this;
}

////////////////////////////////////////////////////////////////////
/// \brief sys::out::operator <<
/// \param obstr
/// \return
///
sys::out& sys::out::operator << (tux::string obstr)
{
    ofs->write(obstr().c_str(),obstr().length());
    return *this;
}

//////////////////////////////////////////////////////////////////
/// \brief sys::out::operator <<
/// \param c
/// \return
///
sys::out& sys::out::operator << (char c)
{
    ofs->write(&c,1);
    return *this;
}


////////////////////////////////////////////////////////////////////////
/// \brief sys::out::operator <<
///     Render the given color enum (color::code) in ansi encoded out.
/// \param clr
/// \return
///
sys::out& sys::out::operator << (ui::color::code clr)
{
    (*ofs) << ui::color::render_rgb(clr);
    return *this;
}


/////////////////////////////////////////////////////////////////////////
/// \brief sys::out::operator <<
/// \param clr
/// \return
///
sys::out& sys::out::operator << (ui::color::pair clr)
{
    (*ofs) << ui::color::render_background_rgb(clr.bg) << ui::color::render_rgb(clr.fg);
    return *this;
}


//////////////////////////////////////////////////////////////////////
/// \brief sys::out::operator <<
///         Transdlates to ansi encoded string the given rem::code enum value
/// \param c
/// \return self& for chaining call
///
sys::out& sys::out::operator << ( rem::code c)
{
    auto [ic,col] = rem::return_code_attributes(c);
    tux::string str;
    str << col << ic << rem::to_string(c);
    (*ofs) <<str();
    return *this;
}

////////////////////////////////////////////////////////////////////////
/// \brief sys::out::operator <<
///         Transdlates to ansi encoded string the given rem::type enum value
/// \param ty
/// \return self& for chaining call
///
sys::out& sys::out::operator << ( rem::type ty)
{
    auto [ic,col] = rem::type_attributes(ty);
    tux::string str;
    str << col << ic << rem::to_string(ty);
    (*ofs) << str();
    return *this;
}

sys::out& sys::out::operator << (ui::cxy xy)
{
    tux::string str;
    (*ofs) << (std::string)xy;
    return *this;
}

sys::out& sys::out::operator << (ui::whinfo z)
{
    tux::string str;
    str << z;
    (*ofs) << (std::string)z;
    return *this;
}

sys::out& sys::out::operator << (ui::rectangle r)
{
    tux::string str;
    str << r;
    (*ofs) << (std::string)r;
    return *this;
}

// sys::out& sys::out::operator << (ui::string2d s2d)
// {
//     tux::string str;
//     (*ofs) << s2d.win;
//     return *this;
// }
//

sys::out& sys::out::operator << (rem::fn f)
{
    tux::string str;

    switch (f) {
        case rem::fn::endl:
            (*ofs) << '\n';
            //            switch (appbook::format()) ///@todo create sys::format(); directly instead.
            //            {
            //                case color::format::ansi256:
            //                    input("\n");
            //                    break;
            //                case color::format::html:
            //                    input("<br />");
            //                    break;
            //            }
            return *this;
        case rem::fn::stamp: {
            /*
                     * %d %f ou %d %m %y
                     * %r %t %h %m %s
                     * %a {monday} %a {mon}
                     * %b {june}   %b {jun}
                */


            //std::chrono::zoned_time date{"america/toronto", std::chrono::system_clock::now()};
            //const auto tp{std::chrono::system_clock::now()};
            //auto txt{tux::string::now("{:%h:%m:%s}", tp)};
            auto [ic, a] = rem::function_attributes(rem::fn::stamp);

            str << a.fg << glyph::data[ic] <<color::reset << tux::string::now("%H:%M:%S");
            (*ofs) << str() << " ";
            return *this;
        }

        case rem::fn::file:
            (*ofs) << location.file_name();
            return *this;
        case rem::fn::line:
        {
            auto [ggg, ccolors] = rem::function_attributes(rem::fn::line);
            str << ccolors << location.line() << color::reset;
            (*ofs)  << "line #" << str();
            return *this;
        }
        case rem::fn::weekday: {
            auto [ic, a] = rem::function_attributes(rem::fn::weekday);
            //auto today{std::chrono::system_clock::now()};
            str << a.fg << tux::string::now("%A");
            (*ofs) << str();
            return *this;
        }

        case rem::fn::day : {
            auto [ic, a] = rem::function_attributes(rem::fn::day);
            //auto today{std::chrono::system_clock::now()};
            str << a.fg << tux::string::now("%d");
            (*ofs) << str();
            return *this;
        }

        case rem::fn::month: {
            auto [ic, a] = rem::function_attributes(rem::fn::month);
            //auto today{std::chrono::system_clock::now()};
            str << a.fg << tux::string::now("%M");
            (*ofs) << str();
            return *this;
        }
        case rem::fn::monthnum: {
            auto [ic, a] = rem::function_attributes(rem::fn::month);
            //auto today{std::chrono::system_clock::now()};
            str << a.fg <<  tux::string::now("%b");
            (*ofs) << str();
            return *this;
        }
        case rem::fn::year: {
            auto [ic, a] = rem::function_attributes(rem::fn::year);
            //auto today{std::chrono::system_clock::now()};
            tux::string acc;
            acc << /*utf::glyph::data[ic] <<*/ a.fg << tux::string::now("%Y");
            (*ofs) <<acc();
            return *this;
        }
        case rem::fn::func:
        {
            auto [gh, colors] = rem::function_attributes(rem::fn::func);
            str << colors << location.function_name() << color::reset << "\n";
            (*ofs) << str() << std::endl;
        }
            break;
        default: break;
    }
    return *this;
}


sys::out& sys::out::endl()
{
    *ofs << std::endl;
    return *this;
}




/////////////////////////////////////////////////////////////
/// \brief sys::out::operator <<
///      Cheat Trick that outputs newline in the working file.
/// \return
///
sys::out& sys::out::operator << (out&)
{
    *ofs << std::endl;
    return *this;
}



#pragma endregion leaf_out

#pragma region exceptions.

///////////////////////////////////////////////////////////////////
/// \brief sys::exception::operator []
///  std::exception based builder.
/// \param el
/// \return
///
sys::exception sys::exception::operator[](const sys::out& el)
{
    _log_ = el;
    return *this;
}


const char* sys::exception::what() const noexcept
{
    if(!_log_.text().empty()) _msg_ = _log_.text();
    else _msg_ = "sys::exception::what() : string buffer was already consumed see the above text.";
    return _msg_.c_str();
}


#pragma endregion exceptions.

////////////////////////////////////////////////////////////////////////////////////
/// \brief sys::error
///     Create a disgnostic::out for output to the specified sys::file.
/// \param hdnl index number of the files list.
/// \param src implicit
/// \return  sys::out instance
///
sys::out sys::error(sys::file::handle hndl, std::source_location &&src)
{
    return{sys::files[hndl].fileptr, rem::type::err, std::move(src)};
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief sys::status
/// \param src
/// \return
///
sys::out sys::status(file::handle h, std::source_location &&src)
{
    return{sys::files[h].fileptr, rem::type::status, std::move(src)};
}

sys::out sys::warning     (sys::file::handle h, std::source_location&& src){
    return {sys::files[h].fileptr,rem::type::warning, std::move(src)}; // NOLINT(*-move-const-arg)
}

sys::out sys::fatal       (sys::file::handle h, std::source_location&& src){
    return {sys::files[h].fileptr,rem::type::fatal, std::move(src)};// NOLINT(*-move-const-arg)
}

sys::out sys::except      (sys::file::handle h, std::source_location&& src){
    return {sys::files[h].fileptr,rem::type::except, std::move(src)};// NOLINT(*-move-const-arg)
}

sys::out sys::message     (sys::file::handle h, std::source_location&& src){
    return {sys::files[h].fileptr,rem::type::message, std::move(src)};// NOLINT(*-move-const-arg)
}

sys::out sys::write       (sys::file::handle h, std::source_location&& src){
    return {sys::files[h].fileptr,rem::type::output, std::move(src)};// NOLINT(*-move-const-arg)
}

sys::out sys::debug       (sys::file::handle h, std::source_location&& src){
    return {sys::files[h].fileptr,rem::type::debug, std::move(src)};// NOLINT(*-move-const-arg)
}

sys::out sys::info        (sys::file::handle h, std::source_location&& src){
    return {sys::files[h].fileptr,rem::type::info, std::move(src)};// NOLINT(*-move-const-arg)
}

sys::out sys::comment     (sys::file::handle h, std::source_location&& src){
    return {sys::files[h].fileptr,rem::type::comment, std::move(src)};// NOLINT(*-move-const-arg)
}

sys::out sys::syntax      (sys::file::handle h, std::source_location&& src){
    return {sys::files[h].fileptr,rem::type::syntax, std::move(src)};// NOLINT(*-move-const-arg)
}

sys::out sys::test        (sys::file::handle h, std::source_location&& src){
    return {sys::files[h].fileptr,rem::type::test, std::move(src)};// NOLINT(*-move-const-arg)
}

sys::out sys::interrupted (sys::file::handle h, std::source_location&& src){
    return {sys::files[h].fileptr,rem::type::interrupted, std::move(src)};// NOLINT(*-move-const-arg)
}

sys::out sys::aborted     (sys::file::handle h, std::source_location&& src){
    return {sys::files[h].fileptr,rem::type::aborted, std::move(src)};// NOLINT(*-move-const-arg)
}

sys::out sys::segfault    (sys::file::handle h, std::source_location&& src){
    return {sys::files[h].fileptr,rem::type::segfault, std::move(src)};// NOLINT(*-move-const-arg)
}

sys::out sys::log         (sys::file::handle h, std::source_location&& src){
    return {sys::files[h].fileptr,rem::type::book, std::move(src)};// NOLINT(*-move-const-arg)
}

rem::code sys::close(file::handle hindex)
{
    return sys::files[hindex].close();
}

rem::code sys::close_all()
{
    for(auto f : sys::files){
        f.close();
    }
    sys::files.clear();
    return rem::code::ok;
}




/////////////////////////////////////////////////////////////////////////////////////////
/// \brief sys::open
///     Creates a new file for a subleaf logger.
/// \param file_id
/// \return
///
std::optional<sys::file::handle> sys::new_file(const std::string &file_id)
{
    if(file_id.empty()){
        return {0};
    }

    tux::string filename;
    filename << file_id << ".dlog";
    auto * f = new std::ofstream(filename().c_str(),std::ios_base::trunc);
    if(!f->is_open()){
        delete f;
        return {};
    }
    sys::files.emplace_back(file_id, f);
    auto n = sys::files.size()-1;

    // auto w = files[n].name.length();
    // auto h = 3;
    // tux::string title;
    // title << "sys output file: " << fs::current_path().string() << '/' << filename << " | #" << n; cadre()[cadre::Vertical];
    // auto canva = vchar::pad::create({static_cast<int>(title.len())+5,3},{color::white,color::r});
    // canva->clear();
    // canva->draw_frame();
    // *canva << cxy{1,1} << title();
    // canva->home();
    // auto l = sys::comment(static_cast<int>(files.size()-1));
    // for (int i=0;i<canva->geometry.dwh.h; i++)
    // {
    //     canva->gotoxy(cxy{0,i});
    //     l << vchar::render_line(canva->cursor_ptr, canva->geometry.dwh.w)<<l;
    // }
    return {n};
}


rem::code sys::use_default(sys::file::handle h)
{
    if (h < 0 || h >= sys::files.size()) return rem::code::rejected;
    sys::_default_file_handle = h;
    return rem::code::accepted;
}
