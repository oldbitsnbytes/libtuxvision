#include "label_dc.h"

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


label_dc::label_dc(element* parent_element, const std::string&text): element(parent_element, "Label",0), _text(text)
{
    _colors_element_id = "Label";
    uistatus_bits = uistatus::Normal;
}


label_dc::~label_dc()
{
    _text.clear();
}


rem::code label_dc::draw()
{
    element::draw();
    auto* paint = begin_paint();
    paint->clear();
    //paint->home();
    *paint << ui::cxy {2,0} << _text;
    // if (component_bits & component::PrefixGlyph)
    // {
    //     paint->set_glyph(_prefix_glyph);
    //     paint->move_to(0,0);
    //     paint->draw_glyph();
    // }
    // if (component_bits & component::SuffixGlyph)
    // {
    //     paint->set_glyph(_suffix_glyph);
    // }
    end_paint(paint);

    return rem::code::done;
}


rem::code label_dc::update()
{
    return element::update();
}


rem::code label_dc::set_prefix_glyph(glyph::type glf)
{
    component_bits |= component::PrefixGlyph;
    _prefix_glyph = glf;
    return rem::code::accepted;
}


rem::code label_dc::set_suffix_glyph(glyph::type glf)
{
    component_bits |= component::SuffixGlyph;
    _suffix_glyph = glf;
    return rem::code::accepted;
}


rem::code label_dc::set_text(const std::string&text)
{
    _text = text;
    return rem::code::accepted;
}

} // namespace tux::vision
