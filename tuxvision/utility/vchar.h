/******************************************************************************************
*   Copyright (C) ...,2025,... by Serge luxsier                                           *
 *   serge.luxsier@oldbitsnbytes.club / luxsier.serge@gmail.com                           *
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
 *   make it  restricted private license.                                                 *
 ******************************************************************************************/



 #pragma once


#include <tuxvision/utility/glyphes.h>
#include <tuxvision/utility/cadres.h>
#include <tuxvision/utility/rem.h>
#include <memory>
#include <vector>
#include <tuxvision/utility/geometry.h>


using namespace tux::integers;


namespace tux::ui
{



class console;
/**
 *
 */
struct vchar
{
    u32 d{0x8003A020};
    using string = std::vector<vchar>;
    using iterator = vchar::string::iterator;
    /*!
    *  \brief console rectangular region for read and write to the console files: [ #0, #1, #2 ].
    *
    */
    static constexpr u32 CharMask  = 0x800000FF;
    static constexpr u32 FGMask    = 0x0000FF00;
    static constexpr u32 BGMask    = 0x00FF0000;
    static constexpr u32 CMask     = 0x00FFFF00;
    static constexpr u32 ColorsMask= 0x00FFFF00;
    static constexpr u32 AttrMask  = 0xFF000000;
    static constexpr u32 UtfMask   = 0xB10000FF;
    static constexpr u32 StyleMask = 0x0E000000;
    // ==============u===attributes==========================
    static constexpr u32 UGlyph    = 0x01000000;
    static constexpr u32 Underline = 0x02000000;
    static constexpr u32 Stroke    = 0x04000000;
    static constexpr u32 Blink     = 0x08000000;
    static constexpr u32 Accent    = 0x10000000;
    static constexpr u32 Frame     = 0x20000000;
    static constexpr u32 TRGB      = 0x40000000;
    /// This will make use of true-color RGB command instead of the re-routed terminal GUI colors profile
    static constexpr u32 ASCII     = 0x80000000;
    static constexpr u32 UTFBITS   = Accent | Frame | UGlyph;
    // ==============Bit Shift ============================
    static constexpr int FGShift = 0x08;
    static constexpr int BGShift = 0x10;
    static constexpr int ATShift = 0x18;

    struct pad
    {
        // --- CONFIGS AND INTERNAL DATA--------------
        color::pair     colors{};
        ui::rectangle   geometry{};
        vchar::string   dc{};           ///< Using one-dimension array of vchar as a two-dimension pad bloc.
        rem::code       state{rem::code::empty};
        vchar::iterator cursor{};
        using shared = std::shared_ptr<vchar::pad>;
        ~pad();

        // --- IO STUFF ------------------------------

        [[nodiscard]] int       width() const {return geometry.dwh.w;}
        [[nodiscard]] int       height() const {return geometry.dwh.h;}
        static shared           create(const ui::whinfo& _dim, color::pair _colours);
        void                    clear();
        rem::code               copy(vchar::pad& pad_dc, rectangle inner_area);
        vchar::iterator         home();
        void                    set_foreground_color(color::code fg);
        void                    set_background_color(color::code bg);
        void                    set_colors(color::pair cp);
        color::code             fg() const;
        color::code             bg() const;
        rectangle               operator&(const rectangle&rhs) const;
        rectangle               operator/(const rectangle&rhs) const;
        vchar::string::iterator operator[](ui::cxy xy);
        vchar::pad&             operator*();

        bool         operator ++();    ///< ++geometry (++geometry.cursor.x)
        bool         operator ++(int); ///< geometry++ (++geometry.cursor.y)
        bool         operator --();    ///< --geometry (--geometry.cursor.x)
        bool         operator --(int); ///< geometry-- (++geometry.cursor.x)
        vchar::iterator set_position(ui::cxy xy);
    };
    vchar() = default;
    explicit vchar(U32 Ch);
    explicit vchar(vchar* C);
    vchar(vchar& C){d = C.d;}
    vchar(const vchar& C){d = C.d;}
    vchar(color::pair cc);
    ~vchar() = default;
    vchar& set_fg(color::code fg_);
    vchar& set_bg(color::code bg_);
    vchar& set_attributes(U32 Ch);
    //vchar& set_colors(color::pair&&Ch);
    // vchar& ResetAttributes(vchar::value_type bits_);
    vchar& set_colors(const color::pair&CP);
    vchar& operator=(U32 Ch);
    vchar& operator=(const vchar& Ch) = default;
    vchar& operator=(const U32* Ch);
    vchar& operator=(char Ch);
    vchar& operator=(int Ch)
    {
        d = Ch;
        return *this;
    };
    [[nodiscard]] std::string get_utf_string() const;
    [[nodiscard]] color::code     foreground() const;
    [[nodiscard]] color::code     background() const;
    [[nodiscard]] color::pair     colors() const;
    [[nodiscard]] glyph::type     icon_id() const;
    [[nodiscard]] accent_fr::type accent_id() const;
    [[nodiscard]] cadre::index    frame_id() const;
    [[nodiscard]] char     ascii() const;
    vchar&  operator*() { return *this; }
    [[nodiscard]] uint16_t attributes() const;
    vchar& operator <<(glyph::type gl);
    vchar& operator <<(accent_fr::type ac);
    vchar& operator << (cadre::index fr);
    vchar& operator <<(color::pair cp);
    vchar& operator <<(vchar Ch)
    {
        d = Ch.d;
        return *this;
    }
    static std::string render_line(vchar::string::iterator _it, std::size_t count);

    vchar&     operator <<(char Ch);
    [[nodiscard]] std::string render_colors() const;
    explicit    operator std::string() const;
    [[nodiscard]] std::string details() const;
};
}// namespace tux::ui
