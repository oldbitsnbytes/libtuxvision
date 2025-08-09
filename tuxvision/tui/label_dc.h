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

class label_dc : public element
{
    CLASSNAME_DERIVED(label_dc)
    std::string _text{};
    glyph::type _prefix_glyph{};
    glyph::type _suffix_glyph{};

public:
    label_dc():element(){};
    label_dc(element* parent_element, const std::string& text);
    ~label_dc() override;

    rem::code draw() override;
    rem::code update() override;

    rem::code set_prefix_glyph(glyph::type glf);
    rem::code set_suffix_glyph(glyph::type glf);
    rem::code set_text(const std::string& text);

};

} // namespace tux::vision
