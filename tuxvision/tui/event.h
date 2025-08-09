//
// Created by Serge Lussier on 2025-07-18.
//

//#ifndef EVENT_H
//#define EVENT_H
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
#include <variant>

namespace tux::ui
{

class event
{

public:


    enum class type:u8{ none,cmd,keyinput,mice }event_type{type::none};
    using value = u64;
    static constexpr value None              = 0x0000000000000000;
    static constexpr value Ok                = 0x0000000000000001;
    static constexpr value Cancel            = 0x0000000000000002;
    static constexpr value Dismiss           = 0x0000000000000004;
    static constexpr value Yes               = 0x0000000000000008;
    static constexpr value No                = 0x0000000000000010;
    static constexpr value Help              = 0x0000000000000020;
    static constexpr value Close             = 0x0000000000000040;
    static constexpr value Save              = 0x0000000000000080;
    static constexpr value Load              = 0x0000000000000100;
    static constexpr value New               = 0x0000000000000200;
    static constexpr value Open              = 0x0000000000000400;
    static constexpr value Edit              = 0x0000000000000800;
    static constexpr value Delete            = 0x0000000000001000;
    static constexpr value Run               = 0x0000000000002000;
    static constexpr value Stop              = 0x0000000000004000;
    static constexpr value Pause             = 0x0000000000008000;
    static constexpr value Resume            = 0x0000000000010000;
    static constexpr value Done              = 0x0000000000020000;
    static constexpr value Abort             = 0x0000000000040000;
    static constexpr value Retry             = 0x0000000000080000;
    static constexpr value Ignore            = 0x0000000000100000;
    static constexpr value Quit              = 0x0000000000200000;
    static constexpr value About             = 0x0000000000400000;
    static constexpr value Exit              = 0x0000000000800000;
    static constexpr value Reset             = 0x0000000001000000;
    static constexpr value Apply             = 0x0000000002000000;
    static constexpr value Key               = 0x0000000004000000;
    static constexpr value Mouse             = 0x0000000008000000;
    static constexpr value Resize            = 0x0000000010000000;
    static constexpr value Focus             = 0x0000000020000000;
    static constexpr value Unfocus           = 0x0000000040000000;
    static constexpr value MouseMove         = 0x0000000080000000;
    static constexpr value MouseDown         = 0x0000000100000000;
    static constexpr value MouseUp           = 0x0000000200000000;
    static constexpr value MouseWheel        = 0x0000000400000000;
    static constexpr value MouseDrag         = 0x0000000800000000;
    static constexpr value MouseClick        = 0x0000001000000000;
    static constexpr value MouseDoubleClick  = 0x0000002000000000;
    static constexpr value Terminate         = 0x0000004000000000;
    struct command
    {
        event::value evt{0};

        command() = default;
        explicit command(value cmd_bits) : evt(cmd_bits) {}
        //command(command&&) = default;
        //command& operator=(command&&) noexcept = default;


        //static std::string_view name(int cn);
        [[nodiscard]] std::string_view name() const;


        rem::action status{rem::action::end};
    };

    //...

    using keybinding = std::map<std::string_view,event::value>;

    static keybinding keybindings;
    static size_t push(io::kbhit&& kev);
    static size_t push(io::mouse&& mev);
    static size_t push(ui::event::command&& cmd);


    event() = default;
    explicit event(io::kbhit&& kev);
    explicit event(io::mouse&& mev);
    explicit event(event::command&& mev);


    template<typename T> [[nodiscard]] bool is() const
    {
        if ((event_type == type::keyinput) && std::is_same<io::kbhit,T>())
            return true;
        if ((event_type == type::mice) && (std::is_same<io::mouse,T>()))
            return true;
        if ((event_type == type::cmd) && (std::is_same<event::command,T>()))
            return true;
        return false;
    }
    static size_t poll();
    static event pop();
    operator bool() const { return event_type != type::none; }


    rem::action operator()();
    union {
        struct event::command cmd;
        struct io::kbhit      kbhit;
        struct io::mouse      mouse;
    }data{};
    value  evt{0};
    static size_t clear();
    static void purge();
private:
    static std::vector<tux::ui::event> _queue;
    static event translate(io::kbhit kev);
    static std::map<event::value, signals::notify_action<tux::ui::event&>> _event_delegates;
};


} // namespace tux::ui
