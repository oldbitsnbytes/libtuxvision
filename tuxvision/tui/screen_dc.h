//
// Created by oldlonecoder on 25-07-27.
//

//#ifndef SCREEN_DC_H
//#define SCREEN_DC_H
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

#include <tuxvision/utility/sys/sys.h>
#include <tuxvision/utility/io/console.h>
#include <tuxvision/utility/notify.h>
//#include <tuxvision/tui/element.h>
#include <tuxvision/tui/window_dc.h>



namespace tux::vision
{

struct screen_dc
{
    using thread_pool = std::vector<std::thread>;
    static rectangle geometry;
    static vchar::pad::shared backbuffer;
    static element* desk;
    static element* target_xy(ui::cxy screen_xy);

    static size_t push(window_dc* win);
    static void reindex();
    static rem::code update_toplevel(element* elem);
    static rem::code anchor_element(element* elem);
    static rem::code update_element_geometry(element* element);
    static rem::code update_back_element(element* elem);
    static rem::code update_area(rectangle area);
    static rem::code init(const std::string& them_name);
    static rem::code draw();
    static rem::code end();
    static element* desktop();
private:


    //------- Reproduire les ancrages dans le z-order 0 :
    static rem::code element_fit_width(element* element);
    static rem::code element_fit_height(element* element);
    static rem::code element_anchor_right(element* element);
    static rem::code element_anchor_bottom(element* element);
    static rem::code element_anchor_left(element* element);
    static rem::code element_anchor_top(element* element);
    static rem::code element_anchor_center(element* element);
    static rem::code element_anchor_center_x(element* element);
    static rem::code element_anchor_center_y(element* element);
    // ---------------------------------------------------------

    static window_dc::list  _windows;


};

//static rem::code draw(const std::string& elem_id);
//static rem::code draw(element* elem_obj);
//static rem::code draw_frame(const std::string& elem_id); // Sous reserve

} // tux::vision

//#endif //SCREEN_DC_H
