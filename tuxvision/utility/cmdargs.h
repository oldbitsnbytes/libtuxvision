/******************************************************************************************
*   Copyright (C) ...,2025,... by Serge luxsier                                          *
*   serge.luxsier@oldbitsnbytes.club / luxsier.serge@gmail.com                            *
*   ----------------------------------------------------------------------------------   *
*   Unless otherwise specified, all Codes ands files in this project is written          *
*   by the author and owned by the auther (Serge luxsier), unless otherwise specified.   *
*   ----------------------------------------------------------------------------------   *
*   Copyrights from authors other than Serge luxsier also apply here.                    *
*   Open source FREE licences also apply To the Code from the author (Serge luxsier)     *
*   ----------------------------------------------------------------------------------   *
*   Usual GNU FREE GPL-1,2, MIT... or whatever -  apply to this project.                 *
*   ----------------------------------------------------------------------------------   *
*   NOTE : All code source that I am the only author, I reserve for myself, the rights to*
*   make it to restricted private license.                                               *
******************************************************************************************/



#pragma once

#include <tuxvision/utility/notify.h>
#include <memory>


namespace tux::cmd
{
/**
 * @class cmd_switch
 * Represents a command-line switch or option in a parsing system.
 *
 * This class is used to define and manage command-line switches,
 * including their name, description, and parse state.
 *
 * Members:
 * - Stores necessary metadata and state information about a command-line switch.
 *
 * Usage typically involves defining switches, associating them with logic,
 * and handling their state during command-line argument parsing.
 */
struct  cmd_switch
{
    using data = std::vector<std::string_view>;
    using shared = std::shared_ptr<cmd_switch>;
    using array = std::vector<cmd_switch::shared>;
    using iterator = cmd_switch::array::iterator;

// ----------  ------------------------------------------
    std::string            name;
    std::string            switch_char;
    std::string            switch_text;
    std::string            descriptions;
    int                    required { 0 };
    int                    count    { 0 };
//--------------------------------------------------------


    data                   arguments{};
    bool                   enabled{ false };
    signals::notify_action<cmd_switch&> notify_callback;
    bool operator !() { return notify_callback.empty(); }

    template<typename T> void set_handler(T* Obj, rem::action (T::*Fn)(cmd_switch&)){
        notify_callback.connect(Obj, Fn);
    }
    cmd_switch& set_descriptions(const std::string& Txt);
    cmd_switch& set_char(const std::string& Txt);
    cmd_switch& set_long_switch(const std::string& Txt);
    cmd_switch& set_required(int N);

    ~cmd_switch();
};


class  line
{

    cmd_switch::array arguments;
    cmd_switch defaults;

    cmd_switch::iterator A{};
public:
    line() = default;
    ~line();

    template<typename T> void set_default_callback(T* Obj, rem::code (T::*Fn)(cmd_switch&))
    {
        (void)defaults.notify_callback.connect(Obj, Fn);
    }

    cmd_switch::iterator query(std::string_view cmd_switch);
    cmd_switch& operator<<(const cmd_switch& Arg);
    cmd_switch& operator [] (const std::string& ArgName);
    ////sys::Enums::Code InputCmdLineData(int argc, char** argv);
    rem::code     input(const std::vector<std::string_view>& StrArray);
    cmd_switch& add_cmd(const std::string& cId);
    rem::action process();
    bool        empty() const { return arguments.empty(); }
    std::string usage();
};

}
