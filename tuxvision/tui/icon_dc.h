
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

class icon_dc : public element
{
    CLASSNAME_DERIVED(icon_dc)

    glyph::type _glyph{};
public:
    icon_dc():element(){};
    icon_dc(element* parent_element, glyph::type glyph);
    ~icon_dc() override;

    rem::code draw() override;
    rem::code update() override;
    rem::code set_glyph(glyph::type glyph);

};

} // namespace tux::vision
