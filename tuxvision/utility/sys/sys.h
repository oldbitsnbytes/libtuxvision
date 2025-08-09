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


#include <optional>

#include <tuxvision/utility/glyphes.h>
#include <tuxvision/utility/string.h>

#include <source_location>

#include <functional>
#include <tuxvision/utility/geometry.h>
#include <filesystem>
#include <fstream>


using tux::ui::color;
using tux::glyph;
using tux::accent_fr;
using tux::ui::rectangle;
using tux::ui::cxy;
using tux::ui::whinfo;






using namespace tux::integers;
using namespace tux::ui;


///////////////////////////////////////////////////////////////////////
/// \brief The sys class
/// \note IMPORTANT! file outputs have to be managed in specific sys::file instance instead of new individual diagnotic::out instance
class sys
{
public:

    static constexpr int indent_width=4;

    struct  out
    {
        std::ostream* ofs{&std::cout};
        struct header_component
        {
            U8 type  : 1;
            U8 stamp : 1;
            U8 fun   : 1;
            U8 file  : 1;
            U8 line  : 1;
            U8 col   : 1;
            U8 hline : 1;
            U8 vline : 1;
            U8 frame : 1; ///< 1 = enable vline|hline|corners and joints.
            //...
        };
        rem::type type{};
        rem::code code{};
        std::source_location location{};
        // ---------------------------------
        tux::string  text{};
        tux::string  header{};

        header_component _headercomp_{1,1,1,1,1,1,0,0,0};
        out() = default;
    private:
        out(std::ostream* file_ptr, rem::type message, std::source_location&& src={});
        out(std::ostream* file_ptr, rem::type message, sys::out::header_component hc, std::source_location&& src={});
        friend class sys;
    public:
        ~out();

        void init_header();
        out& write();
        out& endl();
        out& operator << (out&);
        out& operator << (const std::string&);
        out& operator << (const char*);
        out& operator << (std::string_view);
        out& operator << (tux::string luxstr);
        out& operator << (char c);
        out& operator << (color::code clr);
        out& operator << (color::pair clr);
        out& operator << (rem::code c);
        out& operator << (rem::type ty);
        out& operator << (rem::fn f);
        out& operator << (glyph::type f);
        out& operator << (cxy xy);
        out& operator << (whinfo z);
        out& operator << (rectangle r);
        //out& operator << (string2d s2d);

        template<typename T> out& operator << (const T& v);
    };


    struct file{
        using handle = int; ///< straight vector index.
        std::string name{}; ///< filename
        std::ostream *fileptr{nullptr}; ///< Either a file or the stdout file stream ( std::cout ).
        int page_width{80}; ///< For the frame line.
        using list = std::vector<sys::file>; ///< storage location of the  of sys::file list.
        rem::code close();

        int inden{0};

        // void init_header();
        // file& write();
        // file& endl();
        // file& operator << (out&);
        // file& operator << (const std::string&);
        // file& operator << (const char*);
        // file& operator << (std::string_view);
        // file& operator << (tux::string luxstr);
        // file& operator << (char c);
        // file& operator << (color::code clr);
        // file& operator << (color::pair clr);
        // file& operator << (rem::code c);
        // file& operator << (rem::type ty);
        // file& operator << (rem::fn f);
        // file& operator << (glyph::type f);
        // file& operator << (cxy xy);
        // file& operator << (size z);
        // file& operator << (rectangle r);
        // file& operator << (string2d s2d);

        // template<typename T> file& operator << (const T& v);


    };
    // sys::push(current sys::file is 'A');
    //...auto & out = sys::select(sys::handle B);
    // ...
    // sys::error() << " error text..." << out; // .. << out; = new line and fluxh to out file...
    // diangostic::pop(previous sys::file is now back to 'A');

    sys()=delete;
    ~sys() = default;


    static sys::out error       (sys::file::handle h=0,std::source_location&& src = std::source_location::current());
    static sys::out status      (sys::file::handle h=0,std::source_location&& src = std::source_location::current());
    static sys::out warning     (sys::file::handle h=0,std::source_location&& src = std::source_location::current());
    static sys::out fatal       (sys::file::handle h=0,std::source_location&& src = std::source_location::current());
    static sys::out except      (sys::file::handle h=0,std::source_location&& src = std::source_location::current());
    static sys::out message     (sys::file::handle h=0,std::source_location&& src = std::source_location::current());
    static sys::out write       (sys::file::handle h=0,std::source_location&& src = std::source_location::current());
    static sys::out debug       (sys::file::handle h=0,std::source_location&& src = std::source_location::current());
    static sys::out info        (sys::file::handle h=0,std::source_location&& src = std::source_location::current());
    static sys::out comment     (sys::file::handle h=0,std::source_location&& src = std::source_location::current());
    static sys::out syntax      (sys::file::handle h=0,std::source_location&& src = std::source_location::current());
    static sys::out test        (sys::file::handle h=0,std::source_location&& src = std::source_location::current());
    static sys::out interrupted (sys::file::handle h=0,std::source_location&& src = std::source_location::current());
    static sys::out aborted     (sys::file::handle h=0,std::source_location&& src = std::source_location::current());
    static sys::out segfault    (sys::file::handle h=0,std::source_location&& src = std::source_location::current());
    static sys::out log         (sys::file::handle h=0,std::source_location&& src = std::source_location::current());
    //...
    static rem::code close(sys::file::handle hindex=-1);
    static rem::code close_all();
    static std::optional<sys::file::handle> new_file(const std::string& file_name);
    static rem::code use_default(sys::file::handle h=-1);
    static sys::file::handle _default_file_handle; ///< std::cout by default.
    static sys::file& get(sys::file::handle h);
    class exception :  public std::exception
    {
    public:

        explicit exception():std::exception() { _e_ = true; }
        explicit exception(const char *txt) { _msg_ = txt; _e_ = false; }
        explicit exception(const std::string &txt) { _msg_ = txt; _e_ = false; }

        ~exception() noexcept override {
            _msg_.clear();
        }

        const char* what() const noexcept override;

        sys::exception operator[](const sys::out& el);
    private:
        bool _e_;
        mutable std::string _msg_;
        sys::out _log_;
    };


    struct  Test
    {
        std::string name{};
        template<typename T> rem::code exec(const std::string& sub_test_name, std::function<std::pair<rem::code, T>(sys::Test&)> lambda );

        Test(const std::string& test_name):name(test_name){}
        ~Test() { name.clear(); }

    };

private:
    static file::list files;
};


#include "sys.inc"
