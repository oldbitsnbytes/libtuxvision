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


#include <stack>
#include <tuxvision/utility/glyphes.h>

namespace tux
{

using namespace integers;
using ui::color;


/**
 *
 */
struct  cadre
{
    color::pair colors;     ///< colors of the box::frame


/*!
 * @brief 1v x 4h matrix for addressing Frame components
 * @note  : Source : https://github.com/ArthurSonzogni/FTXUI/blob/main/src/ftxui/screen/string.cpp
 */
    struct  frame_matrix
    {
        uint8_t Left : 2;
        uint8_t Top  : 2;
        uint8_t Right: 2;
        uint8_t Down : 2;
        uint8_t Round: 1;

        bool operator < (cadre::frame_matrix RHS) const;
    };

    struct  frame_data
    {
        glyph::T TL; ///< Top Left
        glyph::T TR; ///< Top Right
        glyph::T BL; ///< Bottom Left
        glyph::T BR; ///< Bottom Right
        glyph::T HL; ///< Horizontal Line
        glyph::T VL; ///< Vertical Line
        glyph::T CVH;///< Cross Vertical-Horizontal
        glyph::T CRL;///< Cross Right-towards-Left
        glyph::T CLR;///< Cross Left-towards-Right
        glyph::T CTD;///< Cross Top-towards-Down
        glyph::T CDT;///< Cross Down-towards-Top
        cadre::frame_data& operator = (cadre::frame_matrix FrameModel);
    }frame{ "╭", "╮", "╰", "╯", "─", "│", "┼", "┤", "├", "┬", "┴" }; ///< By ...default, hardcode the ...default model.
/*           ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃
             ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┗━━{x,x,x,0,0}
             ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┗━━━━━━━{x,0,x,x,0}
             ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┗━━━━━━━━━━━━{0,x,x,x,0}
             ┃    ┃    ┃    ┃    ┃    ┃    ┃    ┗━━━━━━━━━━━━━━━━━{x,x,0,x,0}
             ┃    ┃    ┃    ┃    ┃    ┃    ┗━━━━━━━━━━━━━━━━━━━━━━{x,x,x,x,0}
             ┃    ┃    ┃    ┃    ┃    ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━{0,x,0,x,0}
             ┃    ┃    ┃    ┃    ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━{x,0,x,0,0}
             ┃    ┃    ┃    ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━{x,x,0,0,1}
             ┃    ┃    ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━{0,x,x,0,1}
             ┃    ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━{x,0,0,x,1}
             ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━{0,0,x,x,1}

*/
    cadre();
    cadre::frame_matrix _model_{2,2,2,2,0}; ///< (default) Frame model to apply to frame_data.
    static std::map<std::string_view, cadre::frame_matrix> s_matrix_data;
    const char** data[11] = {&frame.TL, &frame.TR,&frame.BL,&frame.BR,&frame.HL,&frame.VL,&frame.CVH,&frame.CRL,&frame.CLR,&frame.CTD,&frame.CDT};
    enum index : int
    {
        TopLeft=0, TopRight, BottomLeft, BottomRight, Horizontal, Vertical, Cross, VerticalTowardsLeft, VerticalTowardsRight, TopTowardsDown, DownTowardsUp
    };

    static std::stack<cadre::frame_matrix> s_fm;
    static void push(cadre::frame_matrix FrameModel);
    static void pop();

    cadre& operator=(cadre::frame_matrix Model);
    // .Frame[Mem & CharMask]; Enum: Cadre::TopLeft

    /**
     * @brief Retrieves the string representation of a frame component at the given index.
     *
     * This operator allows access to specific components of the frame by their index,
     * returning their respective string representation.
     *
     * @param Ix The index of the frame component to retrieve. It must be a valid value
     *           of the `cadre::index` enumeration.
     * @return std::string The string representation of the frame component at the given index.
     */
    std::string operator [](cadre::index Ix)
    {
        //const char* -> std::string(const char*); // Creates a deep copy and that is what we want.
        // if(Ix > DownTowardsUp)
        //     throw rem::exception() [rem::fatal() << "cadres::index oob (" << Ix << ")!"];
        return *data[Ix];
    }
};


}
