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

#include <tuxvision/utility/sys/sys.h>
#include <tuxvision/utility/object.h>
#include <tuxvision/utility/cmdargs.h>

namespace tux
{

class  app_base : public object
{
    CLASSNAME_DERIVED(app_base)
protected:
    tux::string::view_list _args{};
    tux::string::view_list _env{};
    cmd::line              _proc{};
    signals::notify_action<sys::exception&, app_base&> on_exception{"exception signal"};
    signals::notify_action<sys::exception&, app_base&> on_terminate_request{"terminate request signal"};

    sys::file::handle _dlog{0};
    static void sig_crash(int);
    static void sig_aborted(int);
    static void sig_interrupted(int);

    std::map<const std::string, sys::file::handle> _leaf_handles{};

public:
    struct task
    {
        std::function<rem::code()> func;
        using list = std::vector<task>;
        std::mutex mutex{};
        std::condition_variable cv{};
        bool done{false};
        rem::code operator()(std::function<rem::code()> func);
    };



    app_base(const std::string& app_name, tux::string::view_list&& arguments, tux::string::view_list&& env={});
    app_base():object(){}

    ~app_base() override;

    virtual rem::code run() = 0; ///< app_base is an abstract base app class.
    virtual rem::code setup();
    rem::code add_file(const std::string& diag_name);
    virtual rem::code terminate(rem::type reason);
    virtual rem::code system_signals();
    cmd::line& command_line() { return _proc; }


protected:
    static app_base& app();

};

} // tux
