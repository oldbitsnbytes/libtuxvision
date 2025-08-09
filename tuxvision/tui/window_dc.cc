
#include "window_dc.h"

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






namespace tux::vision
{


window_dc::window_dc(const std::string&elem_id, windowtype::value winbits): element(nullptr, elem_id, winbits|windowtype::Floating|windowtype::Toplevel)
{
    uistatus_bits = uistatus::Normal;
    component_bits |= component::Frame;
    _colors_element_id = "Element";
}



window_dc::~window_dc()
{
    ;
}


rem::code window_dc::draw()
{
    return element::draw();
}


rem::code window_dc::draw_frame()
{
    return element::draw_frame();
}


rem::code window_dc::update()
{
    return element::update();
}


rectangle window_dc::client_area()
{
    return element::client_area();
}


rem::code window_dc::update_child(element* child_element)
{
    return element::update_child(child_element);
}


void window_dc::set_colors(color::pair cp)
{
    element::set_colors(cp);
}

} // namespace tux::vision
