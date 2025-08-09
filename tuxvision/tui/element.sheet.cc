//
// Created by Serge Lussier on 2025-07-12.
//
#include <tuxvision/tui/element.h>



namespace tux::vision
{

element::sheet::sheet(element* parent_element, color::pair cc, rectangle rect): parent(parent_element),
colors(cc), area(rect? rect: parent_element->_geometry.tolocal())
{
    cursor = parent->_dc->set_position({0,0});
    auto l = sys::debug(1); l << "sheet's dimensions:" << color::aquamarine3 << area << color::r << l;
}


element::sheet::~sheet()
{
    ;
}


element::sheet& element::sheet::operator<<(const std::string&_str)
{
    for (auto c:_str)
    {
        *cursor << colors << c;
        if (!++(*this))
        {
            auto l = sys::warning(1); l << rem::code::oob << " reached as the string is longer than the area." << l;
            return *this;
        }
    }
    return *this;
}


element::sheet& element::sheet::operator<<(glyph::type _glyph)
{
    *cursor << colors << _glyph;
    return *this;
}


element::sheet& element::sheet::operator<<(color::code _color)
{
    if (_color == color::reset) _color = parent->_colors.fg;
    colors.fg = _color;
    return *this;
}


element::sheet& element::sheet::operator<<(color::pair _colors)
{
    if (_colors.fg == color::reset) _colors.fg = parent->_colors.fg;
    if (_colors.bg == color::reset) _colors.bg = parent->_colors.bg;
    colors = _colors;
    return *this;
}


element::sheet& element::sheet::operator<<(cadre::index _frameindex)
{
    *cursor << colors << _frameindex;
    ++(*this);
    return *this;
}


element::sheet& element::sheet::operator<<(ui::cxy xy)
{
    if (!area.goto_xy(xy))
    {
        auto l = sys::error(1); l << parent->pretty_id() << rem::code::oob << " : request coord:" << xy << "; within " << area << l;
        return *this;
    }

    cursor = parent->dc()[area.a+xy];
    return *this;
}


element::sheet& element::sheet::operator<<(const vchar::pad&blk)
{
    throw sys::exception()[ sys::error(1) << rem::code::notimplemented];
    return *this;
}


element::sheet& element::sheet::operator<<(char c)
{
    *cursor << c;
    ++(*this);
    return *this;
}


/**
 * Clears the specified rectangle area within the sheet, filling it with the specified color pair.
 *
 * @param r The rectangle area to clear. If the rectangle is empty or invalid, it defaults to the local area of the sheet.
 *          The provided rectangle will be bounded to the sheet's local area.
 * @param cp The color pair to fill in the rectangle area.
 *
 * @return rem::code::done if the operation is successfully completed.
 *         rem::code::rejected if the rectangle is invalid after bounding or has no area to clear.
 */
rem::code element::sheet::clear(ui::rectangle r, color::pair cp)
{
    if (!r) r = area.tolocal();
    else
    {
        r = r & area.tolocal();
        if (!r) return rem::code::rejected;
    }
    for (int y=0; y < r.height(); y++)
        std::fill_n(parent->dc()[r.a+ui::cxy{0,y}], r.dwh.w, vchar(cp) << ' ');

    return rem::code::done;
}


bool element::sheet::operator++()
{
    if (++area)
    {
        cursor = parent->dc()[area.a+area.cursor];
        return true;
    }
    return false;
}


bool element::sheet::operator++(int)
{
    if (area++)
    {
        cursor = parent->dc()[area.a+area.cursor];
        return true;
    }
    return false;
}


bool element::sheet::operator--()
{
    if (--area)
    {
        cursor = parent->dc()[area.a+area.cursor];
        return true;
    }
    return false;
}


bool element::sheet::operator--(int)
{
    if (area--)
    {
        cursor = parent->dc()[area.a+area.cursor];
        return true;
    }
    return false;
}


/**
 * Draws a frame around the specified rectangular area on the sheet, using designated frame characters.
 * The frame includes corners, horizontal lines, and vertical lines created based on the area dimensions.
 *
 * This method assumes the cursor is initially positioned at the specified starting point within the parent display context.
 * The frame elements include:
 * - Top-left, top-right, bottom-right, and bottom-left corner characters.
 * - Horizontal lines formed at the top and bottom edges, excluding corners.
 * - Vertical lines formed at the left and right edges, excluding corners.
 *
 * @return rem::code::done if the frame is successfully drawn.
 */
rem::code element::sheet::draw_frame()
{
    cursor = parent->dc()[area.a];
    auto l = sys::debug(1); l << "draw_frame:" <<color::aquamarine3<<  area << color::r << l;
    home();
    (*this) << cadre::TopLeft
         << area.top_right() << cadre::TopRight
         << area.bottom_right() << cadre::BottomRight
         << area.bottom_left() << cadre::BottomLeft;


    gotoxy(ui::cxy{1,0}); // Real assign from here.
    std::fill(cursor,cursor + area.dwh.w-2, vchar(colors) << cadre::Horizontal);
    gotoxy(ui::cxy{1,area.b.y});
    std::fill(cursor,cursor + area.dwh.w-2, vchar(colors) << cadre::Horizontal);

    for(int y=1; y < area.dwh.h-1; y++)
        *this << ui::cxy{0,y} << cadre::Vertical << ui::cxy{area.b.x,y} << cadre::Vertical;

    return rem::code::done;
}


/**
 * Draws a horizontal line on the sheet from the current cursor position, using the defined character style.
 *
 * @param w The width of the line to draw. If set to a negative value, the line will span the width
 *          of the sheet's local area.
 *
 * @return rem::code::done if the line is successfully drawn.
 */
rem::code element::sheet::draw_line(int w)
{
    if (w<0) w = area.dwh.w;
    std::fill_n(cursor,w, vchar(colors) << cadre::Horizontal);
    return rem::code::done;
}



void element::sheet::set_pos(ui::cxy xy)
{
    if (!area.goto_xy(xy))
    {
        auto l = sys::error(1); l << rem::code::oob << " : request coord:" << xy << "; within " << area.tolocal() << l;
        return;
    }
    cursor = parent->dc()[area.a+xy];
}


void element::sheet::clear()
{
    cursor = parent->dc()[area.a];
    home();
    std::fill_n(cursor,area.dwh.w, vchar(colors) << ' ');
}


void element::sheet::sync_cursor()
{
    const auto delta = static_cast<int>(cursor - parent->dc().dc.begin());
    const ui::cxy xy = {
        delta  % parent->_geometry.dwh.w - area.a.x, delta / parent->_geometry.dwh.h - area.a.y
    };
    area.goto_xy(xy);

}


void element::sheet::home()
{
    cursor = parent->dc()[area.a];
    area.goto_xy({0,0});
}


rem::code element::sheet::gotoxy(ui::cxy xy)
{
    if (!area.goto_xy(xy))
    {
        auto l = sys::error(1); l << rem::code::oob << " : request area:" << xy << "; within " << area.dwh << l;
        return rem::code::rejected;
    }
    cursor = parent->dc()[area.a+xy];
    return rem::code::accepted;
}


void element::sheet::set_foreground_color(ui::color::code fg)
{
    if (fg==color::reset)
        fg = parent->_colors.fg;
    colors.fg = fg;
}


void element::sheet::set_background_color(ui::color::code bg)
{
    if (bg==color::reset)
        bg = parent->_colors.bg;
    colors.bg = bg;
}


void element::sheet::set_colors(ui::color::pair cp)
{
    if (cp.fg==color::reset)
        cp.fg = parent->_colors.fg;
    colors.fg = cp.fg;
    if (cp.bg==color::reset)
        cp.bg = parent->_colors.bg;
    colors.bg = cp.bg;
}


color::code element::sheet::fg() const
{
    return colors.fg;
}


color::code element::sheet::bg() const
{
    return colors.bg;
}


rectangle element::sheet::operator&(const rectangle&rhs)
{
    return area & rhs;
}


rectangle element::sheet::operator/(const rectangle&rhs)
{
    return area / rhs;
}


vchar::string::iterator element::sheet::operator[](ui::cxy xy)
{
    cursor = parent->dc()[xy+area.a];
    return cursor;
}


}
