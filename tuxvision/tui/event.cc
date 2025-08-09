//
// Created by Serge Lussier on 2025-07-18.
//

#include "event.h"
#include <tuxvision/utility/io/console.h>
////////////////////////////////////////////////////////////////////////////////////////////
//   Copyright (C) ...,2025,... by Serge Lussier
//   serge.lussier@oldbitsnbytes.club / lussier.serge@gmail.com
//   ----------------------------------------------------------------------------------
//   Unless otherwise specified, code and files in this project are written
//   by the author and owned by the author (Serge Lussier), unless otherwise specified.
//   ----------------------------------------------------------------------------------
//   Copyrights from authors other than Serge Lussier also apply here.
//   Open source FREE licenses also apply to the code from the author (Serge Lussier)
//   ----------------------------------------------------------------------------------
//   Usual GNU FREE GPL-1,2, MIT... or whatever -  apply to this project.
//   ----------------------------------------------------------------------------------
//   NOTE : All source code that I am the only author, I reserve for myself, the rights to
//   make it to restricted private license.
////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------






namespace tux::ui
{

std::map<event::value,signals::notify_action<tux::ui::event&>> event::_event_delegates{};

event::keybinding event::keybindings{};


std::vector<tux::ui::event> event::_queue{};



std::map<std::string_view,u64> commands_name={
    {"None",                0x0000000000000000},
    {"Ok",                  0x0000000000000001},
    {"Cancel",              0x0000000000000002},
    {"Yes",                 0x0000000000000004},
    {"No",                  0x0000000000000008},
    {"Help",0x0000000000000010},
    {"Close",0x0000000000000020},
    {"Save",0x0000000000000040},
    {"Load",0x0000000000000080},
    {"New",0x0000000000000100},
    {"Open",0x0000000000000200},
    {"Edit",0x0000000000000400},
    {"Delete",0x0000000000000800},
    {"Run",0x0000000000001000},
    {"Stop",0x0000000000002000},
    {"Pause",0x0000000000004000},
    {"Resume",0x0000000000008000},
    {"Done",0x0000000000010000},
    {"Abort",0x0000000000020000},
    {"Retry",0x0000000000040000},
    {"Ignore",0x0000000000080000},
    {"Quit",0x0000000000100000},
    {"About",0x0000000000200000},
    {"Exit",0x0000000000400000},
    {"Reset",0x0000000000800000},
    {"Apply",0x0000000001000000},
    {"Key",0x0000000002000000},
    {"Mouse",0x0000000004000000},
    {"Resize",0x0000000008000000},
    {"Focus",0x0000000010000000},
    {"Unfocus",0x0000000020000000},
    {"MouseMove",0x0000000040000000},
    {"MouseDown",0x0000000080000000},
    {"MouseUp",0x0000000100000000},
    {"MouseWheel",0x0000000200000000},
    {"MouseDrag",0x0000000400000000},
    {"MouseClick",0x0000000800000000},
    {"MouseDoubleClick",0x0000001000000000},
    {"Terminate",0x0000002000000000}

};


std::string_view event::command::name() const
{
    for (auto [name,value] : commands_name) {
        if (value == evt)
            return name;
    }
    return "None";
}



rem::action event::operator()()
{
    if (auto fn = event::_event_delegates[evt]; !fn.empty())
        return fn(*this);
    return rem::action::dismiss;
}


size_t event::push(io::kbhit&&kev)
{
    auto l = sys::message(1); l << color::lime << " - " << glyph::rust_crab << " loop: pushing " <<color::cyan <<  glyph::keyboard << color::r <<'|' << color::fuchsia << glyph::keyboard << color::r << " input:" << l;
    event::_queue.emplace_back(std::move(event(std::move(kev)))); // NOLINT(*-move-const-arg)
    return event::_queue.size();
}


size_t event::push(io::mouse&&mev)
{
    auto l = sys::message(1); l << color::lime << " - " << glyph::rust_crab << " loop: pushing " <<color::cyan <<  glyph::keyboard << color::r <<'|' << color::fuchsia << glyph::mouse << color::r << " input:" << l;
    event::_queue.emplace_back(std::move(event(std::move(mev))));
    return event::_queue.size();
}


size_t event::push(ui::event::command&&cmd)
{
    auto l = sys::message(1); l << color::lime << " - " << glyph::rust_crab << " loop: pushing " <<color::cyan <<  glyph::bolt << color::r <<'|' << color::fuchsia << glyph::bolt << color::r << " input:" << l;
    event::_queue.emplace_back(std::move(event(std::move(cmd)))); // NOLINT(*-move-const-arg)
    return event::_queue.size();
}


event::event(io::kbhit&&kev): event_type(event::type::keyinput)
{
    data.kbhit = kev;
    ///@todo Translate to command if applicable
}


event::event(io::mouse&&mev): event_type(event::type::mice)
{
    data.mouse = mev;
    ///@todo Translate to command if applicable
}


event::event(event::command&& cmd): event_type(event::type::cmd)
{
    data.cmd = cmd;
    ///@todo Translate to command if applicable
}



size_t event::poll()
{
    auto l = sys::debug(1); l << "polling io::console input:" << l;
    auto ret = io::console::poll_in();
    if (!ret)
        return 0;
    do
    {
        auto cev = io::console::events().front();
        io::console::events().pop_front();
        if (cev.is<io::kbhit>())
        {
            //io::kbhit kev = cev.data.kev;
            // Translate the input key here.
            event::_queue.push_back(event::translate(cev.data.kev));
            continue;
        }

        if (cev.is<io::mouse>())
        {
            event::_queue.emplace_back(std::move(cev.data.mev));
        }
    }while (!io::console::events().empty());

    // @todo pop console event.
    l << " returning " << color::yellow << event::_queue.size() << color::r << " events" << l;
    //...
    return event::_queue.size();
}


event event::pop()
{
    if (event::_queue.empty()) return {};
    auto ret = event::_queue.front();
    event::_queue.erase(event::_queue.begin());
    return ret;
}


size_t event::clear()
{
    auto s = event::_queue.size();
    event::_queue.clear();
    return s;
}


void event::purge()
{
    event::_queue.clear();
}


event event::translate(io::kbhit kev)
{
    auto l = sys::debug(1); l << "translating kbhit:" << l;
    switch (kev.mnemonic)
    {
        case io::kbhit::CHARACTER:
            return event(std::move(kev));  // NOLINT(*-move-const-arg)
        case io::kbhit::ESCAPE:
            l << "ESCAPE -> event::command::Cancel:" << l;
            return event(event::command(event::Cancel));
   //...

        default: break;
    }
    return {};
}

} // namespace tux::ui
