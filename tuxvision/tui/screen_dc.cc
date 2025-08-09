//
// Created by oldlonecoder on 25-07-27.
//

#include <tuxvision/tui/screen_dc.h>

#include <ranges>

#include "application.h"
#include "tuxvision/utility/colors.h"

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
//   make it restricted private license.
////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------






namespace tux::vision
{


rectangle screen_dc::geometry{};
vchar::pad::shared screen_dc::backbuffer{nullptr};
element* screen_dc::desk{nullptr};
window_dc::list  screen_dc::_windows{};
rectangle dirty_area{};

std::mutex render_guard{};

/**
 * Initializes the screen display context by setting up its geometry and
 * creating a backbuffer with the specified color pair.
 *
 * @param cp A color::pair object specifying the foreground and background
 *           colors to be used for the backbuffer.
 * @return A code indicating the operation result:
 *         - rem::code::rejected if the geometry could not be determined or is invalid.
 *         - rem::code::accepted if all went ok....
 */
rem::code screen_dc::init(const std::string& theme_name)
{
    io::console::begin();
    screen_dc::geometry = io::console::geometry();

    //... check geometry before going further
    if (!screen_dc::geometry)
        return rem::code::rejected;

    screen_dc::desk = new element(nullptr,"tuxvision::desktop",0);
    screen_dc::backbuffer = vchar::pad::create(screen_dc::geometry.dwh, {color::black, color::grey3});
    screen_dc::backbuffer->clear();

    screen_dc::desk->set_theme(theme_name);
    screen_dc::desk->set_geometry(screen_dc::geometry);

    io::console::init_stdinput();
    return rem::code::accepted;
}


rem::code screen_dc::draw()
{
    screen_dc::desk->draw();
    for (auto* w : screen_dc::_windows)
        w->draw();
    io::console::render(desk->_dc.get(),desk->_geometry.a);
    for (auto*w : screen_dc::_windows)
        io::console::render(w->_dc.get(),w->_geometry.a);
    return rem::code::accepted;
}


rem::code screen_dc::end()
{
    delete screen_dc::desk;
    io::console::end();
    for (auto* w: screen_dc::_windows) delete w;
    screen_dc::_windows.clear();
    return rem::code::accepted;
}


element* screen_dc::desktop()
{
    return screen_dc::desk;
}


rem::code screen_dc::anchor_element(element* elem)
{
    if (!!screen_dc::update_element_geometry(elem))
    {
        elem->_dc->geometry.a = elem->_geometry.a;
        auto l = sys::info(1); l << elem->pretty_id() << " new geometry:" << color::yellow << elem->_geometry << color::r << l;
        return rem::code::accepted;
    }
    auto l = sys::info(1); l << elem->pretty_id() << " anchor request rejected" << l;
    return rem::code::notimplemented;
}


/**
 * Updates the geometry of the specified screen display context element based on
 * its anchor attributes, adjusting its position and dimensions accordingly.
 *
 * @param elem A pointer to the element whose geometry is to be updated. The element
 *             must not be null and should have valid anchor attributes defined.
 * @return A code indicating the operation result:
 *         - rem::code::accepted if the geometry adjustment is successfully applied.
 *         - rem::code::notimplemented if certain anchor configurations or adjustments
 *           are not yet implemented.
 */
rem::code screen_dc::update_element_geometry(element* elem)
{
    // If fit width then we only can fit bottom or top.
    auto l = sys::info(1); l << elem->pretty_id() << " anchor bits value:" << color::yellow << elem->anchor_bits << color::r << l;
    if (elem->anchor_bits & anchor::Width)
    {

        screen_dc::element_fit_width(elem);
        if (elem->anchor_bits & anchor::Top)
            screen_dc::element_anchor_top(elem);
        else if (elem->anchor_bits & anchor::Bottom)
            screen_dc::element_anchor_bottom(elem);
        return rem::code::accepted;
    }


    // If fit height then we only  can fit right, left or center_x
    if (elem->anchor_bits & anchor::Height)
    {
        screen_dc::element_fit_height(elem);
        if (elem->anchor_bits & anchor::Left)
            screen_dc::element_anchor_left(elem);
        else if (elem->anchor_bits & anchor::Right)
            screen_dc::element_anchor_right(elem);
        //@todo Center-H
        return rem::code::accepted;
    }

    //@todo ... :
    if (elem->anchor_bits & anchor::Center)
    {
        auto l = sys::info(1); l << elem->pretty_id() << " anchor center of the screen:" << l;
        return screen_dc::element_anchor_center(elem);
        //...
    }

    //...
    return rem::code::notimplemented;
}


/**
 * Updates the backbuffer element of the screen display context by merging the dirtied
 * areas and copying required portions from the element's display context to the backbuffer.
 *
 * @param elem A pointer to the element to be updated in the backbuffer. The element must
 *             have a valid dirty area defined. Null pointers or elements without a dirty
 *             area will cause the update to be rejected.
 * @return A code indicating the operation result:
 *         - rem::code::rejected if the element is null, has no dirty area, or no valid
 *           rectangles were computed for update.
 */
rem::code screen_dc::update_back_element(element* elem)
{
    if (!elem || !elem->dirty_area)
        return rem::code::rejected;

    rectangle r = screen_dc::geometry & elem->dirty_area+elem->_geometry.a;
    if (!r)
        return rem::code::rejected;

    render_guard.lock();
    dirty_area |= r;
    //...
    backbuffer->copy(*elem->_dc, r - elem->_geometry.a);
    // ...
    render_guard.unlock();
    return rem::code::accepted;
}


/**
 * Adjusts the width of the given child element to fit the width of the screen's
 * display context geometry while maintaining its original position and height.
 *
 * @param child_element A pointer to the child element whose width needs to be adjusted.
 *                      The element must have a valid geometry and must not be null.
 * @return A code indicating the operation result:
 *         - rem::code::accepted if the width adjustment was applied successfully.
 *         - rem::code::rejected if the display context geometry is not defined or the input is invalid.
 */
rem::code screen_dc::element_fit_width(element* child_element)
{
    if (!screen_dc::geometry) return rem::code::rejected;
    child_element->set_geometry({{0,child_element->geometry().a.y},ui::whinfo{screen_dc::geometry.dwh.w,child_element->_geometry.dwh.h}});
    return rem::code::accepted;
}


rem::code screen_dc::element_fit_height(element* child_element)
{
    if(!screen_dc::geometry) return rem::code::rejected;
    child_element->set_geometry({{child_element->geometry().a.x,0},ui::whinfo{child_element->_geometry.dwh.w,screen_dc::geometry.dwh.h}});
    return rem::code::accepted;
}


rem::code screen_dc::element_anchor_right(element* child_element)
{
    if(!screen_dc::geometry) return rem::code::rejected;
    child_element->_geometry.set_x(screen_dc::geometry.dwh.w - child_element->_geometry.dwh.w);
    return rem::code::accepted;
}


rem::code screen_dc::element_anchor_bottom(element* child_element)
{
    if(!screen_dc::geometry) return rem::code::rejected;
    child_element->_geometry.set_y(screen_dc::geometry.dwh.h - child_element->_geometry.dwh.h);
    return rem::code::accepted;
}


rem::code screen_dc::element_anchor_left(element* child_element)
{
    if(!screen_dc::geometry) return rem::code::rejected;
    child_element->_geometry.set_x(0);
    return rem::code::accepted;
}


rem::code screen_dc::element_anchor_top(element* child_element)
{
    if(!screen_dc::geometry) return rem::code::rejected;
    child_element->_geometry.set_y(0);
    return rem::code::accepted;
}


rem::code screen_dc::element_anchor_center(element* child_element)
{
    if(!screen_dc::geometry) return rem::code::rejected;
    child_element->_geometry.set_x((screen_dc::geometry.dwh.w - child_element->_geometry.dwh.w) / 2);
    child_element->_geometry.set_y((screen_dc::geometry.dwh.h - child_element->_geometry.dwh.h) / 2);
    return rem::code::accepted;
}


rem::code screen_dc::element_anchor_center_x(element* child_element)
{
    if(!screen_dc::geometry) return rem::code::rejected;
    child_element->_geometry.set_x((screen_dc::geometry.dwh.w - child_element->_geometry.dwh.w) / 2);
    return rem::code::accepted;
}


rem::code screen_dc::element_anchor_center_y(element* child_element)
{
    if(!screen_dc::geometry) return rem::code::rejected;
    child_element->_geometry.set_y((screen_dc::geometry.dwh.h - child_element->_geometry.dwh.h) / 2);
    return rem::code::accepted;
}




element* screen_dc::target_xy(ui::cxy screen_xy)
{
    auto l = sys::debug(1);l << " begin query target from: " << color::cadetblue2 << screen_xy << l;
    if (screen_dc::_windows.empty())
    {
        l << "no top-level elements to target. Dismissing this query." << l;
        return nullptr;
    }
    // std::range approaches from clang-tidy's suggestion:
    for (auto & _window : std::ranges::reverse_view(screen_dc::_windows))
    {
        l << "hit test :" << _window->pretty_id() << " " << _window->_geometry << l;
        if (_window->_geometry[screen_xy])
            return _window->target_xy(screen_xy-_window->_geometry.a);
    }
    l << "no target found. Dismissing this query." << l;
    return nullptr;
}


size_t screen_dc::push(window_dc* win)
{
    screen_dc::_windows.push_back(win);
    screen_dc::reindex();
    return screen_dc::_windows.size();
}


void screen_dc::reindex()
{
    size_t z{1}; // Zero (0) is the screen elements ( the backbuffer )
    for (auto* w : screen_dc::_windows)
        w->_zorder = z++;
}


rem::code screen_dc::update_area(rectangle area)
{
    return rem::code::notimplemented;
}

rem::code screen_dc::update_toplevel(element* toplevel_elem)
{
    // - Get the first free and ready screen_dc::task
    //   Set mutex blocking
    // - call its task::operator()([&]() ->rem::code {
    //                // ...
    //                io::console::render(toplevel_elem->_dirty_rect);
    //                //...
    //                return rem::code::done;
    //            }
    // - unblock mutex
    return rem::code::done;
}


} // namespace tux::vision
