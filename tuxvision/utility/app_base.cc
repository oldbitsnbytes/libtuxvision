//
// Created by oldbitsnbytes on 25-03-08.
//

#include <tuxvision/utility/app_base.h>
#include <csignal>

namespace tux
{

static app_base* _app_{nullptr};


void app_base::sig_crash(int)
{
    auto log = sys::interrupted(_app_->_dlog);
    log << "terminating with segfault signal" << log;
    _app_->terminate(rem::type::segfault);
    exit(0);
}


void app_base::sig_aborted(int)
{
    auto log = sys::aborted(_app_->_dlog);
    log << "terminating with abort signal" << log;
    _app_->terminate(rem::type::aborted);
    exit(0);
}


void app_base::sig_interrupted(int)
{
    auto log = sys::interrupted(_app_->_dlog);
    log << "terminating with interruption signal" << log;
    _app_->terminate(rem::type::interrupted);
    exit(0);
}


app_base::app_base(const std::string& app_name, tux::string::view_list&& arguments, tux::string::view_list&& env_list):object(nullptr, std::move(app_name))
{
    _args = std::move(arguments);
    _env = std::move(env_list);
    _app_ = this;
}


app_base::~app_base()
{
    //sys::status() << "destroy; disconnecting all notify signals.\n";
    on_exception.disconnect_all();
    on_terminate_request.disconnect_all();
    //...

    _args.clear();
}


/**
 * Sets up the application environment by initializing the logging mechanism
 * and preparing the system for handling signals.
 *
 * This function performs the following operations:
 * - Creates or opens a "leaf" file named "application".
 * - Updates the `_dlog` variable with the new file handle.
 * - Stores the file handle in the `_leaf_handles` map for later reference.
 * - Sets the created "leaf" file as the default for logging.
 * - Logs a success message indicating that the application leaf file
 *   has been successfully opened.
 * - Configures system signals.
 *
 * @return rem::code::done if the setup completes successfully.
 */
rem::code app_base::setup()
{
    auto h = sys::new_file("application");
    _dlog = *h;
    _leaf_handles["application"] = _dlog;
    sys::use_default(_dlog);
    auto l = sys::status(_dlog);
    l << "Successfully opened the application logs file" << l;
    system_signals();
    return rem::code::done;
}


rem::code app_base::add_file(const std::string& diag_name)
{
    auto h = sys::new_file(diag_name);
    if (!h)
    {
        //...
        auto log = sys::error(_dlog);
        log << "failed to add leaf file identified by '" << color::hotpink4 <<  diag_name << color::r << "'.\n" << color::red4 << "rejected." << color::r << log;
        return rem::code::rejected;
    }
    _leaf_handles[diag_name] = *h;

    return rem::code::accepted;
}



rem::code app_base::terminate(rem::type _reason)
{
    auto l = sys::status(1); l << "terminate reason: " << _reason <<l;
    //sys::use_default(0);
    return rem::code::terminate;
}


rem::code app_base::system_signals()
{
    std::signal(SIGSEGV, &app_base::sig_crash      );
    std::signal(SIGABRT, &app_base::sig_aborted    );
    std::signal(SIGINT,  &app_base::sig_interrupted);
    return rem::code::done;
}


// rem::action app_base::commands()
// {
//     auto r = command_line().input(_args);
//     if(!!r)
//         return _proc.process();

//     auto log = sys::error();
//     log << " command line arguments processing failed with: " << r << log;
//     return rem::action::end;
// }

app_base &app_base::app()
{
    return *_app_;
}


} // tux
