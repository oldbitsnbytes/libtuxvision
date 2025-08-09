

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



#include "application.h"
#include <tuxvision/utility/io/console.h>
#include <tuxvision/tui/screen_dc.h>

namespace tux::vision
{



application::application(const std::string&app_name, tux::string::view_list&& arguments, tux::string::view_list&& env): app_base(app_name, std::move(arguments), std::move(env)){}


application::~application()
{
    ;
}



rem::code application::setup()
{
    return app_base::setup();
}


rem::code application::terminate(rem::type reason)
{
    app_base::terminate(reason);
    event::purge();
    screen_dc::end();
    return rem::code::done;
}


rem::code application::process_event(event&evt)
{
    switch (evt.event_type)
    {
        case event::type::keyinput:
            return process_keyinput(evt);
        case event::type::mice:
            return process_mouse(evt);
            break;
        case event::type::cmd:
            return process_command(evt);

        default:break;
    }
    return rem::code::done;
}


rem::code application::setup_ui()
{
    //...
    return screen_dc::init("C128");
}


rem::code application::process_keyinput(event&evt)
{
    if (evt.data.kbhit.mnemonic == io::kbhit::ESCAPE)
    {
        return rem::code::terminate;
    }
    return rem::code::accepted;
}


rem::code application::process_mouse(event&evt)
{
    return rem::code::accepted;
}


rem::code application::process_command(event&evt)
{
    return rem::code::accepted;
}



rem::code application::run()
{
    while (event::poll())
    {
        auto evt = event::pop();
        if (process_event(evt) == rem::code::terminate)
            return rem::code::terminate;
        //...
    }
    return rem::code::terminate;
}


} // namespace tux::vision
