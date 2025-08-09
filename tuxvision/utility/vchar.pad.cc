#include <tuxvision/utility/sys/sys.h>
#include <tuxvision/utility/vchar.h>


namespace tux::ui
{

// ---------------------------------------------------------------------------------------------------------------------





vchar::iterator vchar::pad::set_position(ui::cxy _pos)
{
    if (!geometry.goto_xy(_pos))
    {
        state = rem::code::oob;
        return cursor;
    }

    state = rem::code::accepted;
    return dc.begin() + geometry.width()*_pos.y + _pos.x;
}




/**
 *  @brief Write std::string at the current position.
 *
 * ...
 *  @return ref to self for chaining calls.
 *
vchar::pad& vchar::pad::operator<<(const std::string& _str)
{
    if (_str.length() >= (dc.end() - cursor_ptr))
        throw sys::exception()[
            sys::except(1) << rem::type::fatal << rem::code::oob
                << rem::fn::endl << "strlen:" << color::yellow << _str.length() << color::r
                << rem::fn::endl << "buffer length from end() pointer: " << color::yellow << (dc.end() - dc.begin())
                << rem::fn::endl << "logical position of the cursor: " << color::yellow << geometry.cursor << rem::fn::endl
        ];

    for (auto c: _str)
        **cursor_ptr++ << colors << c;
    sync_cursor(); // We must sync internal cursors before further inputs.
    return *this;
}




 *
vchar::pad& vchar::pad::operator<<(glyph::type _glyph)
{
    // if (!++geometry)
    // {
    //     auto l = sys::error(1); l << " vchar::pad::operator<<(glyph::type) : request area:" << geometry.dwh << l;
    //     return *this;
    // }
    **cursor_ptr++ << colors << _glyph;
    return *this;
}




 *
vchar::pad& vchar::pad::operator<<(color::code _color)
{
    if (_color == color::reset)
        _color = colors.fg;

    cursor_ptr->set_bg(_color);
    return *this;
}


vchar::pad& vchar::pad::operator<<(color::pair _colors)
{
    if (_colors.fg == color::reset)
        _colors.fg = colors.fg;
    if (_colors.bg == color::reset)
        _colors.bg = colors.bg;

    cursor_ptr->set_colors(_colors);
    return *this;
}


vchar::pad& vchar::pad::operator<<(cadre::index _frameindex)
{
    **cursor_ptr++ << _frameindex;
    return *this;
}


vchar::pad& vchar::pad::operator<<(ui::cxy xy)
{
    gotoxy(xy);
    // Handle result...
    return *this;
}


vchar::pad& vchar::pad::operator<<(const vchar::pad& blk)
{
    return *this;
}


vchar::pad& vchar::pad::operator<<(char c)
{
    **cursor_ptr << c;
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////
/// \brief vchar::pad::clear
///     Clears sub-area given by \arg r using colors attributes \arg cp
/// \param r
/// \param cp
/// \return accepted if all of the clear went ok, rem::code::rejected otherwise
///
rem::code vchar::pad::clear(ui::rectangle r, color::pair cp)
{
    auto rw = geometry.tolocal() & (r);
    if(!rw){
        auto l = sys::status(); l << rem::code::oob << " : request area:" << r << "; within " << geometry.dwh << l;
        return rem::code::rejected;
    }

    for(int y=0; y < rw.height(); y++){
        if(auto c = set_pos({rw.a.x, rw.a.y+y}); !c)
            return c;
        for(int x=0; x < rw.width(); x++){
            ***this << cp << ' ';
            if(!++*this)
                return rem::code::rejected;
        }
    }
    return rem::code::accepted;
}
*/


/*!
 * @note bypasses natural increment-decrement because this method is the convenient way to advance the internal cursor with our boundaries.
 * @return true if the cursor is within the boundaries  false otherwise.
 * @author Serge luxsier (oldlonecoder)
 */


bool vchar::pad::operator++()
{
    if (++geometry)
    {
        ++cursor;
        return true;
    }
    return false;
}


/*!
 * @note bypass natural increment-decrement because this method is the convenient way to advance the internal cursor with our boundaries.
 * @return true if the cursor is within the boundaries  false otherwise.
 * @author Serge luxsier (oldlonecoder)
 */
bool vchar::pad::operator++(int)
{
    if (++geometry)
    {
        ++cursor;
        return true;
    }
    return false;
}


/*!
 *
 * @note bypass natural increment-decrement because this method is the convenient way to advance the internal cursor with our boundaries.
 * @return true if the cursor is within the boundaries  false otherwise.
 * @author Serge luxsier (oldlonecoder)
 */
bool vchar::pad::operator--()
{
    if (--geometry)
    {
        --cursor;
        return true;
    }
    return false;
}

// rem::code vchar::pad::draw_frame()
// {
//     home();
//     *this<< cadre::TopLeft
//          << geometry.top_right() << cadre::TopRight
//          << geometry.bottom_right() << cadre::BottomRight
//          << geometry.bottom_left() << cadre::BottomLeft;
//
//
//     gotoxy(ui::cxy{1,0}); // Real assign from here.
//     std::fill(cursor_ptr,cursor_ptr + geometry.dwh.w-2, vchar(colors) << cadre::Horizontal);
//     gotoxy(ui::cxy{1,geometry.b.y});
//     std::fill(cursor_ptr,cursor_ptr + geometry.dwh.w-2, vchar(colors) << cadre::Horizontal);
//
//     for(int y=1; y < geometry.dwh.h-1; y++)
//         *this << ui::cxy{0,y} << cadre::Vertical << ui::cxy{geometry.b.x,y} << cadre::Vertical;
//
//     return rem::code::done;
// }
//
//
// rem::code vchar::pad::draw_line(int w)
// {
//     if (w<0)
//     {
//         w = geometry.dwh.w;
//         set_pos({0,geometry.cursor.y});
//     }
//     for(int i=0; i < w; i++)
//         *this << colors << cadre::Horizontal;
//
//     return rem::code::done;
// }
//

/*!
 *
 * @return true if the cursor is within the boundaries  false otherwise.
 * @author Serge luxsier (oldlonecoder)
 */
bool vchar::pad::operator--(int)
{
    if (--geometry)
    {
        --cursor;
        return true;
    }
    return false;
}



vchar::pad::~pad()
{
    dc.clear();
    geometry={};
}


vchar::pad::shared vchar::pad::create(const ui::whinfo& _dim, color::pair _colours)
{
    auto p  = std::make_shared<vchar::pad>();

    p->dc = vchar::string(_dim.area(),vchar(color::pair(_colours)));
    p->colors = _colours;
    p->geometry = {{0,0},_dim};
    p->clear();
    auto l = sys::info(1); l << " vchar::pad size:" << color::yellow << p->geometry.dwh.area() << l;
    return p;
}





//////////////////////////////////////////////////////////////
/// \brief vchar::pad::clear
///        Clears the buffer with the current colors attributes
void vchar::pad::clear()
{
    std::fill_n(dc.begin(),geometry.dwh.area(), vchar(color::pair(colors)));
}


/**
 *  @brief Copies a defined inner area of one pad into another pad.
 *
 *  This function takes a specified inner area from the provided source pad and
 *  copies it into the calling pad instance. It calculates the absolute positions
 *  based on the geometry of the source pad and the destination pad.
 *
 *  @param pad_dc The source pad from which the inner area will be copied.
 *  @param inner_area The rectangular region within the source pad that is to be copied.
 *  @return rem::code::accepted if the operation completes successfully.
 */
rem::code vchar::pad::copy(vchar::pad&pad_dc, rectangle inner_area)
{
    auto rw = inner_area+pad_dc.geometry.a;
    for (int y = 0;y < rw.dwh.h; y++)
        std::copy_n(pad_dc[inner_area.a+ui::cxy{0,y}],rw.dwh.w, (*this)[rw.a+ui::cxy{0,y}]);

    return rem::code::accepted;
}


//////////////////////////////////////////////////////////////////////
/// \brief vchar::pad::home
///     Resets iterator and internal cursor at {0,0};
///
vchar::iterator vchar::pad::home()
{
    geometry.goto_xy({0,0});
    cursor = dc.begin();
    return cursor;
}



////////////////////////////////////////
/// \defgroup pad-colors
///     Just copying into the current colors member vchar::pad::colors
///


///////////////////////////////////////////////////////////////////////////////
/// \brief vchar::pad::set_foreground_color
/// \param fg
/// \group pad-colors
void vchar::pad::set_foreground_color(color::code fg)
{
    colors.fg = fg;
    cursor->set_fg(fg);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief vchar::pad::set_background_color
/// \param bg
/// \group pad-colors
void vchar::pad::set_background_color(color::code bg)
{
    colors.bg = bg;
    cursor->set_bg(bg);
}

///////////////////////////////////////////////////////
/// \brief vchar::pad::set_colors
/// \param cp
/// \group pad-colors
void vchar::pad::set_colors(color::pair cp)
{
    colors = cp;
    cursor->set_colors(cp);
}

////////////////////////////////////////////////////////////
/// \brief vchar::pad::fg
/// \return color code
/// \group pad-colors
/// \note Statusquo sur lire la couleur de la cellule au cursor_ptr ou donner la couleur courante a appliquer
color::code vchar::pad::fg() const { return colors.fg; }

////////////////////////////////////////////////////////////
/// \brief vchar::pad::bg
/// \return
/// \group pad-colors
/// \note Statusquo sur lire la couleur de la cellule au cursor_ptr ou donner la couleur courante a appliquer
///
color::code vchar::pad::bg() const { return colors.bg; }



////////////////////////////////////////////////////////////////////////
/// \brief vchar::pad::operator &
///     Intersect the rectangular geometry of this pad with \arg rhs.
/// \param rhs
/// \return resulting of the intersection.
/// \note \arg rhs origin must be on the same origin of this pad.
///
rectangle vchar::pad::operator &(const rectangle& rhs) const { return geometry & rhs; }


////////////////////////////////////////////////////////////////////////
/// \brief vchar::pad::operator /
///     Intersect the rectangular inner geometry ( origin at {0,0} ) of this pad with \arg rhs.
/// \param rhs
/// \return resulting of the intersection shifted to the inner origin {0,0}.
/// \note \arg rhs must be on the same origin scale of this pad. So the resulting rectangle will have its offset moved to the relative geometry of this pad
///
rectangle vchar::pad::operator /(const rectangle& rhs) const { return geometry / rhs; }


/**
 *
 */
vchar::string::iterator vchar::pad::operator[](ui::cxy xy)
{
    return dc.begin() + (xy.y * geometry.dwh.w) + xy.x;
}



vchar::pad& vchar::pad::operator *() { return *this; }




//-----------------------------------------------------------------------------


}
