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
//----------------------------------------------------------------------------------


#include "element.h"
#include <tuxvision/utility/sys/sys.h>
#include <tuxvision/tui/screen_dc.h>




namespace tux::vision
{

element::element(element* parent_element, const std::string& elem_id, windowtype::value ui_class):object(parent_element,elem_id),windowtype_bits(ui_class)
{
    uistatus_bits = uistatus::Active;
    windowtype_bits = ui_class;
    _colors_element_id = "Element";
}


element::~element()
{
    dirty_area = {};
    _geometry = {};
    // dc is self-managed shared pointer.
}



rem::code element::set_geometry(rectangle r)
{
    if (windowtype_bits&windowtype::Toplevel ||  parent<element>())
    {
        auto p = parent<element>();
        if (p)
            _dc = p->_dc;
    }

    if (!_dc)
        _dc = vchar::pad::create(r.dwh, _colors);

    _geometry = r;
    return rem::code::accepted;
}


element& element::operator<<(int cnd_mnemonic)
{

    return *this;
}


rem::code element::process_event(event&evt)
{
    auto l = sys::info(1); l << pretty_id() <<l;
    return rem::code::accepted;
}


rem::code element::set_anchor(anchor::value ancbits)
{
    anchor_bits |= ancbits;
    auto l = sys::info(1); l << pretty_id() << color::white << color::r << " @" << color::yellow << geometry().a << color::reset <<l;
    element* par = parent<element>();
    if (par)
    {
        l = sys::info(1) << pretty_id() << color::white << color::r << " Request parent to update this child..." << l;
        par->place_child_element(this);
    }
    else
        l = sys::info(1) << pretty_id() << color::white << color::r << " no parent, will invoque screeen_dc::anchor_element(window);..." << l;
    return rem::code::accepted;
}


/**
 * @brief Retrieves the client area of the element in local coordinates.
 *
 * This function transforms the geometry of the element to its local coordinate system
 * and returns it as a rectangle object.
 *
 * @return rectangle representing the client area of the element in local coordinates.
 */
rectangle element::client_area()
{
    return _geometry.tolocal();
}


/**
 * @brief Updates the state of a child element and propagates changes up the hierarchy.
 *
 * This function updates the current element's dirty area with the dirty area of the specified child element.
 * If the current element has a parent of type `element`, the update is propagated recursively up the hierarchy.
 * Otherwise, the changes are pushed to the screen via the top-level display context update.
 *
 * @param child_element Pointer to the child element whose state is being updated.
 * @return rem::code indicating the result of the operation. Typically returns rem::code::done upon successful completion.
 */
rem::code element::update_child(element* child_element)// NOLINT(*-no-recursion)
{
    dirty_area |= child_element->dirty_area;
    if(auto const p = parent<element>(); p != nullptr)
        return p->update_child(this);

    // if (parent<window_manager>() != nullptr)
    // {
    //     return window_manager::instance()->push_update(this);
    // }
    screen_dc::update_toplevel(this);
    dirty_area = {};
    return rem::code::done;
}


/**
 * @brief Sets the color pair for the element and recursively applies it to its children.
 *
 * This function assigns the provided color pair to the current element.
 * It then propagates the same color settings to all child elements
 * that are of type `element` by recursively invoking this function.
 *
 * @param cp The color pair to be applied to the element and its children.
 */
void element::set_colors(color::pair cp)// NOLINT(*-no-recursion)
{
    _colors = cp;
    for (auto* child : m_children)
    {
        if (auto *c = child->as<element>(); c != nullptr)
            c->set_colors(cp);
    }
}


/**
 * @brief Places a child element within the current element with specified padding.
 *
 * This function sets the padding information for the child element
 * and updates its geometry within the container element. The operation
 * results in the return of a status code to indicate success or if the
 * operation is not yet implemented.
 *
 * @param child_element Pointer to the child element to be placed within the current element.
 * @param padding The padding information to be applied around the child element.
 * @return rem::code indicating the result of the operation.
 */
rem::code element::place_child_element(element* child_element, padding_info padding)
{
    _padding = padding;
    if (windowtype_bits & component_bits&component::Frame)
        _padding = {1,1,1,1};

    return update_child_geometry(child_element);
}


rem::code element::set_focus(element* child_element)
{
    ///@todo Change uistatus to focussed, change colors to active|focus, set parent's focussed_child to this then redraw
    return rem::code::notimplemented;
}


rem::code element::set_event_mask(event::value mask)
{
    event_mask = mask;
    return rem::code::accepted;
}


rem::code element::set_position(cxy xy)
{
    _geometry.moveat(xy);
    return rem::code::accepted;
}


/**
 * @brief Updates the geometry of a child element based on its anchor constraints.
 *
 * This function adjusts the position and size of the specified child element
 * according to its defined anchor bits. It handles constraints for width, height,
 * and their respective combinations (e.g., top, bottom, left, right, center).
 * If the child's geometry cannot be updated with the current logic, a "not implemented" code is returned.
 *
 * @param child_element Pointer to the child element whose geometry needs to be updated.
 * @return rem::code indicating the result of the operation. Possible values include:
 *         - rem::code::accepted if the geometry update is completed successfully.
 *         - rem::code::notimplemented if the update logic is not implemented for the given anchor configuration.
 */
rem::code element::update_child_geometry(element* child_element)
{

    // If fit width then we only can fit bottom or top.
    if (child_element->anchor_bits & anchor::Width)
    {

        child_fit_width(child_element);
        if (child_element->anchor_bits & anchor::Top)
            child_anchor_top(child_element);
        else if (child_element->anchor_bits & anchor::Bottom)
            child_anchor_bottom(child_element);
        return rem::code::accepted;
    }


    // If fit height then we only  can fit right, left or center_x
    if (child_element->anchor_bits & anchor::Height)
    {
        child_fit_height(child_element);
        if (child_element->anchor_bits & anchor::Left)
            child_anchor_left(child_element);
        else if (child_element->anchor_bits & anchor::Right)
            child_anchor_right(child_element);
        //@todo Center-H
        return rem::code::accepted;
    }

    //@todo ... :
    if (child_element->anchor_bits & anchor::Center)
    {
        if ((child_element->anchor_bits & anchor::Top) || (child_element->anchor_bits & anchor::Bottom))
        {
            return child_anchor_center_x(child_element);
        }
        else if ((child_element->anchor_bits & anchor::Left) || (child_element->anchor_bits & anchor::Right))
        {
            return child_anchor_center_y(child_element);
        }
        return rem::code::accepted;
    }


    return rem::code::notimplemented;
}


/**
 * @brief Assigns theme colors to the element and its children.
 *
 * This function assigns theme-related colors to the current element
 * based on its state and updates the theme for all its child elements recursively.
 *
 * @param component_id The identifier of the component for which theme colors are assigned.
 * @return rem::code indicating the success or failure of the assignment operation. Typically, rem::code::accepted is returned on success.
 */
rem::code element::assign_theme_colors(std::string_view component_id) // NOLINT(*-no-recursion)
{
    _colors = _element_colors->second[uistatus_bits];
    for (auto* child : m_children)
        if (auto *c = child->as<element>(); c != nullptr)
            c->set_theme(_theme_id);

    return rem::code::accepted;
}


rem::code element::set_components(component::value bits)
{
    component_bits = bits;
    return rem::code::accepted;
}




/**
 * @brief Identifies the target element at a specific coordinate within the element's local space.
 *
 * This function recursively traverses the element's hierarchy, starting with the current element,
 * to find and return the child element that contains the specified local coordinate.
 * If no such child element is found, it returns the current element.
 *
 * @param element_xy The local coordinate relative to this element's coordinate space.
 * @return A pointer to the target element containing the specified coordinate, or nullptr element if no match is found.
 */
element* element::target_xy(ui::cxy element_xy)// NOLINT(*-no-recursion)
{
    auto l = sys::write(1); l << pretty_id() << color::white << color::r << " @" << color::yellow << element_xy << color::reset <<l;
    for (auto o: m_children)
    {
        if (auto* e = o->as<element>(); e != nullptr)
        {
            if (e->_geometry[element_xy])
                if (auto* t = e->target_xy(element_xy-e->_geometry.a); t != nullptr)
                    return t;
        }
    }
    return this;
}


rem::code element::set_theme(std::string_view theme_name)// NOLINT(*-no-recursion)
{
    if (theme_name.empty())
        theme_name = "Default";
    _theme_id = theme_name;
    auto pit = colordb::palettedb.find(_theme_id.data());
    _element_colors = pit->second.find(_colors_element_id.data());

    assign_theme_colors(_colors_element_id.data());

    return rem::code::accepted;
}


rem::code element::draw()// NOLINT(*-no-recursion)
{
    auto p  = begin_paint();
    if (!p) return rem::code::failed;

    p->clear();
    // ... Check ui class bits:
    if (component_bits & component::Frame)
        p->draw_frame();
    end_paint(p);
    for (auto* e:m_children)
        if (auto* c = e->as<element>(); c != nullptr)
            c->draw();
    return rem::code::done;
}


rem::code element::draw_frame()
{
    auto p = begin_paint();
    p->draw_frame();
    return rem::code::done;
}


rem::code element::update()
{
    if (!dirty_area) return rem::code::empty;
    auto p = parent<element>();
    if (p)
        return p->update_child(this);

    //...

    dirty_area = {};

    return rem::code::done;
}

element::sheet invalid_sheet{};

element::sheet* element::begin_paint(rectangle rect)
{
    if (!rect)
        rect = is_toplevel() ?  _geometry.tolocal() : _geometry;
    else
    {
        rect = _geometry.tolocal() & rect;
        if (! rect)
            return nullptr;
    }
    auto p = new element::sheet(this, _colors, rect);
    return p;
}



rem::code element::end_paint(element::sheet *sh)
{
    dirty_area |= sh->area;
    delete sh;
    //.. Dirty pad dc;
    return rem::code::done;
}



rem::code element::child_fit_width(element* child_element)
{
    if (!_dc) return rem::code::rejected;
    child_element->set_geometry(
        {
            ui::cxy{_padding.left,child_element->geometry().a.y},
            ui::whinfo{_geometry.dwh.w-_padding.left-_padding.right,child_element->_geometry.dwh.h}
        });
    return rem::code::accepted;
}


rem::code element::child_fit_height(element* child_element)
{
    if (!_dc) return rem::code::rejected;
    child_element->set_geometry(
        {
            ui::cxy{child_element->geometry().a.x,0},
            ui::whinfo{child_element->_geometry.dwh.w,_geometry.dwh.h-_padding.top-_padding.bottom}
        });
    return rem::code::accepted;
}


rem::code element::child_anchor_right(element* child_element)
{
    if (!_dc) return rem::code::rejected;
    child_element->_geometry.moveat({_geometry.dwh.w - child_element->_geometry.dwh.w - _padding.right,child_element->_geometry.a.y});
    return rem::code::accepted;
}


rem::code element::child_anchor_bottom(element* child_element)
{
    if (!_dc) return rem::code::rejected;
    child_element->_geometry.moveat({child_element->_geometry.a.x,_geometry.dwh.h - child_element->_geometry.dwh.h-_padding.bottom});
    return rem::code::accepted;
}


rem::code element::child_anchor_left(element* child_element)
{
    if (!_dc) return rem::code::rejected;
    child_element->_geometry.moveat({_padding.left,child_element->_geometry.a.y});
    return rem::code::accepted;
}


rem::code element::child_anchor_top(element* child_element)
{
    if (!_dc) return rem::code::rejected;
    child_element->_geometry.moveat({child_element->_geometry.a.x, _padding.top});
    return rem::code::accepted;
}


rem::code element::child_anchor_center(element* child_element)
{
    if (!_dc) return rem::code::rejected;
    child_element->_geometry.moveat(ui::cxy{(_geometry.dwh.w - child_element->_geometry.dwh.w) / 2,
        (_geometry.dwh.h - child_element->_geometry.dwh.h) / 2});
    return rem::code::accepted;
}


rem::code element::child_anchor_center_x(element* child_element)
{
    if (!_dc) return rem::code::rejected;
    child_element->_geometry.moveat(ui::cxy{_geometry.dwh.w - (child_element->_geometry.dwh.w) / 2, child_element->_geometry.a.y});
    return rem::code::accepted;
}


rem::code element::child_anchor_center_y(element* child_element)
{
    if (!_dc) return rem::code::rejected;
    child_element->_geometry.moveat(ui::cxy{child_element->_geometry.a.x,(_geometry.dwh.h - child_element->_geometry.dwh.h) / 2});
    return rem::code::accepted;
}




} // namespace lus::vision
