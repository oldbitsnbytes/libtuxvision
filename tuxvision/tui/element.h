//
// Created by Serge Lussier on 2025-07-10.
//

//#ifndef ELEMENT_H
//#define ELEMENT_H
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


#include <tuxvision/utility/object.h>
#include <tuxvision/utility/sys/sys.h>
#include <tuxvision/utility/vchar.h>
#include <tuxvision/tui/globals.h>

#include <optional>

#include <tuxvision/tui/event.h>

using namespace tux::ui;

namespace tux::vision
{

class element : public object
{
    CLASSNAME_DERIVED(element)

    ui::vchar::pad::shared _dc{nullptr};
    ui::rectangle          _geometry{};
    ui::color::pair        _colors{};
    std::string_view       _theme_id{"Default"};



protected:
    component::value    component_bits{component::None};
    anchor::value       anchor_bits{anchor::None};
    uistatus::value     uistatus_bits{uistatus::Normal};
    windowtype::value   windowtype_bits{windowtype::None};
    element*            focussed_child{nullptr};

    u64                 event_mask{0}; ///< event::value response bits

    colordb::elements::iterator _element_colors{};

public:

    struct sheet
    {
        rectangle          area{};
        color::pair        colors{};
        element*           parent{nullptr};
        vchar::iterator    cursor{};

        sheet() = default;
        sheet(element* parent_element, color::pair cc, rectangle rect={});
        ~sheet();

        element::sheet&         operator << (const std::string& _str);
        element::sheet&         operator << (glyph::type _glyph);
        element::sheet&         operator << (color::code _color);
        element::sheet&         operator << (color::pair _colors);
        element::sheet&         operator << (cadre::index _frameindex);
        element::sheet&         operator << (ui::cxy xy);
        element::sheet&         operator << (const vchar::pad& blk);
        element::sheet&         operator << (char c);
        rem::code               clear(ui::rectangle r, color::pair cp);
        bool                    operator ++();    ///< ++geometry (++geometry.cursor.x)
        bool                    operator ++(int); ///< geometry++ (++geometry.cursor.y)
        bool                    operator --();    ///< --geometry (--geometry.cursor.x)
        bool                    operator --(int); ///< geometry-- (++geometry.cursor.x)
        rem::code               draw_frame();
        rem::code               draw_line(int w=-1);
        explicit                operator bool() const;
        void                    set_pos(ui::cxy xy);
        void                    clear();
        void                    sync_cursor();
        void                    home();
        rem::code               gotoxy(ui::cxy xy);
        void                    set_foreground_color(ui::color::code fg);
        void                    set_background_color(ui::color::code bg);
        void                    set_colors(ui::color::pair cp);
        color::code             fg() const;
        color::code             bg() const;
        rectangle               operator & (const rectangle& rhs);
        rectangle               operator / (const rectangle& rhs);
        vchar::string::iterator operator[](ui::cxy xy);
        element::sheet&         operator *() { return *this; }


        // ------------------ Not yet enabled --------------------
        // rem::code    scroll_up(int nrows);
        // void         scroll_down(int nrows);
        // void         scroll_left(int nrows);
        // void         scroll_right(int nrows);
        // std::pair<ui::size, vchar::string> copy(rectangle r);
        // rem::code                          put(ui::size, const vchar::string& blk, ui::cxy xy);
        // -------------------------------------------------------
    };


    element():object(){};
    element(element* parent_element, const std::string& elem_id, windowtype::value winbits); // ,vision::eclass::value class_bits...)
    ~element() override;
    element* target_xy(ui::cxy element_xy);
    rem::code set_theme(std::string_view theme_name);

    virtual rem::code draw();
    virtual rem::code draw_frame();
    virtual rem::code update();

    vchar::pad& dc() { return *_dc.get(); }

    element::sheet* begin_paint(rectangle rect={});
    rem::code       end_paint(element::sheet* sh);


    rem::code set_geometry(rectangle r);
    [[nodiscard]] const rectangle& geometry() const { return _geometry; }

    [[nodiscard]] bool is_toplevel() const  {return windowtype_bits & windowtype::Toplevel;}

    element& operator << (int cnd_mnemonic);
    virtual rem::code process_event(event &evt);
    rem::code         set_anchor(anchor::value anchor_bits);
    rem::code place_child_element(element* child_element, padding_info padding={});
    virtual rem::code set_focus(element* child_element);

    rem::code set_event_mask(event::value mask);
    rem::code set_position(cxy xy);
    [[nodiscard]] int width() const { return _geometry.dwh.w;}
    [[nodiscard]] int height() const{ return _geometry.dwh.h;}

protected:

    rectangle dirty_area{};
    std::string_view       _colors_element_id{"Element"};

    virtual rectangle client_area();
    virtual rem::code update_child(element* child_element);
    virtual void      set_colors(color::pair cp);

    virtual rem::code update_child_geometry(element* child_element);
    virtual rem::code child_fit_width(element* child_element);
    virtual rem::code child_fit_height(element* child_element);
    virtual rem::code child_anchor_right(element* child_element);
    virtual rem::code child_anchor_bottom(element* child_element);
    virtual rem::code child_anchor_left(element* child_element);
    virtual rem::code child_anchor_top(element* child_element);
    virtual rem::code child_anchor_center(element* child_element);
    virtual rem::code child_anchor_center_x(element* child_element);
    virtual rem::code child_anchor_center_y(element* child_element);



    rem::code assign_theme_colors(std::string_view component_id);
    rem::code set_components(component::value bits);

private:
    friend struct screen_dc;
    friend class element::sheet;
    padding_info _padding{};  ///< When placing child element.
};

} // namespace lus::vision
