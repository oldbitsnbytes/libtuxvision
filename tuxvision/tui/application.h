//
// Created by oldlonecoder on 25-07-20.
//

//#ifndef APPLICATION_H
//#define APPLICATION_H
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


#include <tuxvision/utility/app_base.h>

#include <tuxvision/tui/event.h>

namespace tux::vision
{

class application : public app_base
{

    CLASSNAME_DERIVED(application)

public:

    application(const std::string& app_name, tux::string::view_list&& arguments, tux::string::view_list&& env={});
    ~application() override;

    rem::code run() override;
    rem::code setup()override;
    rem::code terminate(rem::type reason) override;
    virtual rem::code process_event(event& evt);
protected:
    virtual rem::code setup_ui();
    std::thread::id ui_thread_id{};
    virtual rem::code process_keyinput(event& evt);
    virtual rem::code process_mouse(event& evt);
    virtual rem::code process_command(event& evt);
    using thread_pool = std::vector<std::thread>;

};

} // namespace tux::vision


//#endif //APPLICATION_H
