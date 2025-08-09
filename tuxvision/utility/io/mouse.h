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
#include <tuxvision/utility/io/lfd.h>
#include <tuxvision/utility/geometry.h>

namespace tux::io
{


/*!
 * \brief The mouse data struct
 *
 */
struct  mouse
{


    ///////////////////////////////////////////////////////////////////////////////////////
    /// \brief BUTTON_UP
    /// ex.: button.left == mouse::BUTTON_UP;
    static constexpr u8 BUTTON_UP = 0;
    ///////////////////////////////////////////////////////////////////////////////////////
    /// \brief BUTTON_PRESSED
    /// ex.: button.left == BUTTON_PRESSED;
    static constexpr u8 BUTTON_PRESSED = 3;
    ///////////////////////////////////////////////////////////////////////////////////////
    /// \brief BUTTON_DOWN
    /// ex.: button.left == mouse::BUTTON_DOWN;
    static constexpr u8 BUTTON_DOWN = 1;
    ///////////////////////////////////////////////////////////////////////////////////////
    /// \brief BUTTON_RELEASE
    /// ex : button.left == BUTTON_RELEASE;
    static constexpr u8 BUTTON_RELEASE = 2;

    mouse()=default;
    ~mouse() = default;

    mouse(const mouse& cpy) = default;
    mouse(mouse&& mref) noexcept = default;

    mouse& operator=(const mouse& cpy);
    mouse& operator=(mouse&& mref) noexcept = default;

    struct  buttons
    {
        u8 left  : 2; // 0 = up; 1 = down; 2 = pressed event; 3 = released event;
        u8 mid   : 2; // 0 = up; 1 = down; 2 = pressed event; 3 = released event;
        u8 right : 2; // 0 = up; 1 = down; 2 = pressed event; 3 = released event;
        u8 b4    : 2;
        u8 b5    : 2;
        u8 b6    : 2;
        u8 st    : 2; // 0: noevent; 1: pressed; 2: released;
    }button{0,0,0};

    struct 
    {
        u8 shift :1;
        u8 alt   :1;
        u8 win   :1;
    }state{0,0,0};

    ui::cxy pos{};
    ui::cxy dxy{};

    bool left   (){return button.left;}
    bool middle (){return button.mid;}
    bool right  (){return button.right;}

    mouse& operator & (const mouse& mev);
    static rem::code parse(lfd& _fd);
    static mouse prev_mev;
    operator bool();
    std::string operator()();
    static std::string get_direction_arrow(ui::cxy xy);
    bool pressed = false;
private:
    static rem::code parse(bool brel,std::vector<int> args_);
};

} // namespace tux::io
