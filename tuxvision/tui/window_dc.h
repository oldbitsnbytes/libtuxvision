
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


#include <tuxvision/tui/element.h>

namespace tux::vision
{

class window_dc: public element
{

    CLASSNAME_DERIVED(window_dc)

    friend struct screen_dc;
    // All ui that can be managed by window_dc:
    //  [titlebar (caption; window-buttons)]; statusbar; [menubar (master-menu-ui)]; toolbar; etc...
    // --------------------------------------------------------------------------------------------
    size_t _zorder{0};
    using list = std::vector<window_dc*>;
public:
    window_dc():element(){};

    window_dc(const std::string& elem_id, windowtype::value winbits);
    ~window_dc() override;

    rem::code draw() override;
    rem::code draw_frame() override;
    rem::code update() override;


protected:

    rectangle client_area() override;
    rem::code update_child(element* child_element) override;
    void      set_colors(color::pair cp) override;
};


} // NAMESPACE tux::vision
