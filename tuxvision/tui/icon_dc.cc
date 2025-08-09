//
// Created by oldlonecoder on 25-07-28.
//

#include "icon_dc.h"

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



icon_dc::icon_dc(element* parent_element, glyph::type glyph):element(parent_element, "Icon",0), _glyph(glyph)
{
    _colors_element_id = "Icon";
    uistatus_bits = uistatus::Normal;
}


icon_dc::~icon_dc()
{
    ;
}


rem::code icon_dc::draw()
{
    return element::draw();
}


rem::code icon_dc::update()
{
    return element::update();
}


rem::code icon_dc::set_glyph(glyph::type glyph)
{
    _glyph = glyph;
    return rem::code::accepted;
}



} // namespace tux::vision
