/******************************************************************************************
 *   Copyright (C) ...,2025,... by Serge luxsier                                          *
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
 *   make it to restricted private license.                                               *
 ******************************************************************************************/

#pragma once 


#include <tuxvision/utility/vchar.h>
#include <tuxvision/utility/notify.h>
#include <tuxvision/utility/object.h>
#include <tuxvision/utility/geometry.h>
#include <tuxvision/utility/io/kbhit.h>
#include <tuxvision/utility/io/mouse.h>
#include <tuxvision/utility/io/lfd.h>

#include <termios.h>
#include <sys/ioctl.h>
// https://www.xfree86.org/current/ctlseqs.html
#define SET_X10_MOUSE 9
#define SET_VT200_MOUSE 1000
#define SET_VT200_HIGHLIGHT_MOUSE 1001
#define SET_BTN_EVENT_MOUSE 1002
#define SET_ANY_EVENT_MOUSE 1003
// --------------------------------------------

namespace tux::io::console
{


static constexpr u8 st_saved         = 1;
static constexpr u8 st_mouse_enabled = 2;
static constexpr u8 st_activated     = 4;
static constexpr u8 st_raw_set       = 8;
static constexpr u8 st_poll_started  = 0x10;
static constexpr u8 st_poll_closed   = 0x20;
static constexpr u8 st_caret_on      = 0x40;






static constexpr u8 use_mouse               = 1;
static constexpr u8 report_mouse_position   = 2;
static constexpr u8 report_mouse_buttons    = 4;
static constexpr u8 use_double_buffer       = 8;    ///< Also tells console to save and clear the initial screen then restore it at exit.
static constexpr u8 auto_hide_caret         = 0x10; ///< Hide the caret when not on keyboard input prompt.


enum class caret_shapes: u8
{
    def,
    bloc_blink,
    block_steady,
    under_blink,
    under_steady,
    vbar_blink,
    vbar_steady
};


static constexpr auto LINE_WRAP                 = "7";
static constexpr auto CRS_SHAPE                 = "25";
static constexpr auto MOUSE_X10                 = "9 ";
static constexpr auto SET                       = 'h';
static constexpr auto RESET                     = 'l';
static constexpr auto MOUSE_VT200               = "\x1b[?1000";
static constexpr auto MOUSE_VT200_HL            = "\x1b[?1001";
static constexpr auto MOUSE_REPORT_BUTTONS      = "\x1b[?1002";
static constexpr auto MOUSE_REPORT_ANY          = "\x1b[?1003";
static constexpr auto MOUSE_UTF8                = "\x1b[?1005";
static constexpr auto MOUSE_SGR_EXT_MODE        = "\x1b[?1006";
static constexpr auto MOUSE_URXVT_MODE          = "\x1b[?1015";
static constexpr auto MOUSE_SGR_PIXEL_MODE      = "\x1b[?1016";
static constexpr auto ALTERNATE_SCREEN          = "\x1b[?1049";
static constexpr auto CSI                       = "\x1b[";
static constexpr auto CSI_BOLD                  = "\x1b[1m";
static constexpr auto CSI_UNDERLINE             = "\x1b[4m";
static constexpr auto CSI_REVERSE               = "\x1b[7m";
static constexpr auto CSI_STRIKETHROUGH         = "\x1b[35m";
static constexpr auto CSI_UNDERLINE_PREVIOUS    = "\x1b[90m";
static constexpr auto CSI_UNDERLINE_NEXT        = "\x1b[92m";
static constexpr auto CSI_UNDERLINE_INCREMENT   = "\x1b[93m";
static constexpr auto CSI_UNDERLINE_DECREMENT   = "\x1b[94m";


rectangle geometry();
rem::code   enable_mouse();
rem::code   stop_mouse();
rem::code   query_winch();
rem::code   begin();
rem::code   end();

void switch_alternate();
void switch_back();
void clear();
void cursor_off();
void cursor_on();
bool cursor(ui::cxy xy);

rem::code render(vchar::pad* blk, ui::cxy xy={0,0});
rem::code render(ui::cxy xy, vchar::iterator start, int length);
rem::code init_stdinput();

struct  event{
    enum evt {KEV,MEV,WIN,UNDEF}type{evt::UNDEF};
    union {
        mouse mev;
        kbhit kev;
        ui::whinfo win;
    }data{};

    template<typename EVT> bool is(){
        if(type == KEV) return std::is_same<kbhit, EVT>();
        if(type == MEV) return std::is_same<mouse, EVT>();
        if(type == WIN) return std::is_same<ui::whinfo, EVT>();
        return false;
    }
    operator bool() { return type != evt::UNDEF; }

    using queue = std::deque<console::event>;
};


rem::code poll_in();
rem::code enqueu(event&& ev);
console::event::queue& events();
void push_event(console::event&& ev);
rem::action parse_stdin(io::lfd& ifd);
rem::code stdin_proc();
signals::notify_action<rectangle>& term_resize_signal();

} // namespace io::console
