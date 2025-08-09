#include <tuxvision/utility/io/console.h>
#include <csignal>

#include <sys/poll.h>
namespace tux::io::console
{



// local (file) vars:
#pragma region local_vars


struct termios      _saved{},
                    _this{};

ui::rectangle       _geometry{};
ui::color::pair     _colors{};
pollfd              _poll_events[1]{{0,POLL_ERR|POLL_IN|POLL_HUP}};
u8                  _flags{0};
u8                  _state{0};
lfd                 _fd0{};

console::event::queue _events{};


caret_shapes cursor_shape{caret_shapes::def};


signals::notify_action<ui::rectangle> _window_resize_signal{"terminal resize signal notifier"};



#pragma endregion local_vars

void resize_signal(int );

signals::notify_action<rectangle>& term_resize_signal()
{
    return _window_resize_signal;
}


rem::code enqueu(console::event &&ev)
{
    _events.push_back(std::move(ev));
    return rem::code::accepted;
}


console::event::queue & events()
{
    return console::_events;
}


//////////////////////////////////////////////////////////////////////////////////////
/// \brief terminal::push_event
/// \param ev
/// \todo Guard and unlock _events queue with a mutex and signal/notify threads pool with condition variable.
///
void push_event(console::event&& ev)
{
    // lock
    _events.push_back(std::move(ev));
    // unlock
}


/*!
 * @brief Gets the console's dimension.
 * @return rem::code
 */
rem::code query_winch()
{
    winsize win{};
    ioctl(fileno(stdout), TIOCGWINSZ, &win);
    if((!win.ws_col)||(!win.ws_row))
        return rem::code::notexist;

    _geometry = rectangle{{0,0}, ui::whinfo{static_cast<int>(win.ws_col), static_cast<int>(win.ws_row)}};

    // debug or info:
    auto log = sys::info(1); log << " The console size is: ["
    << color::yellow << std::format("{:>3d}x{:<3d}",_geometry.dwh.w,_geometry.dwh.h) << color::reset << "]" << log;

    return rem::code::done;
}



rectangle geometry() { return _geometry; }



rem::code begin()
{
    tcgetattr(STDIN_FILENO, &_saved);
    _this = _saved;

    //new_term.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | IGNCR | IXON );
    _this.c_iflag &= ~(BRKINT | PARMRK | ISTRIP | IGNCR | IXON );
    _this.c_oflag &= ~OPOST;
    _this.c_cflag |= (CS8);
    _this.c_cflag &= ~PARENB;
    _this.c_lflag &= ~(ICANON | ECHO | IEXTEN | TOSTOP | ECHONL);
    _this.c_cc[VMIN] = 0;
    _this.c_cc[VTIME] = 1;
    _this.c_cc[VSTART] = 0;
    _this.c_cc[VSTOP] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &_this);

    //sys::write() << " terminal set to raw mode..." << //sys::endl;

    //::signal(SIGWINCH, &terminal::resize_signal); // --> Must be Handled in the thread/proc/ env
    //if(_flags & terminal::use_double_buffer)
    //    switch_alternate();
    cursor_off();
    query_winch();
    enable_mouse();
    init_stdinput();

    return rem::code::done;
}

rem::code end()
{
    //if(_flags & terminal::use_double_buffer)
    //    switch_back();
    //sys::info() << "closing terminal state:" << //sys::eol;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &_saved);
    //auto l = sys::info(1); l << "cursor on, disabling mouse tracking mode." << //sys::eol;
    cursor_on();
    stop_mouse();
    _fd0.clear();
    _events.clear();
    auto l = sys::info(1); l << color::lime << "Restored initial console state and purged the console::_events queue." << color::z << l;
    return rem::code::done;
}


/**
 *
 */
void switch_alternate()
{
    std::cout << "\x1b[0m\x1b[?1049h" << std::flush;
    _flags |= st_saved;
}

void switch_back()
{
    std::cout << "\x1b[0m\x1b[?1049l" << std::flush;
    _flags &= ~st_saved;
}


/**
 * @brief Set terminal to report the mouse events.

    @note Execute CSI on each parameters because I realized that "\x1b[?1000;1002;...h" is not working.
 *
 */
rem::code enable_mouse()
{
    //std::cout << MOUSE_VT200            << terminal::SET;
    std::cout << MOUSE_REPORT_BUTTONS   << SET;
    //std::cout << MOUSE_REPORT_ANY       << SET << std::flush;
    std::cout << MOUSE_SGR_EXT_MODE     << SET<< std::flush;
    //std::cout << MOUSE_URXVT_MODE       << SET << std::flush;
    _flags |= use_mouse;

    return rem::code::accepted;
}

rem::code stop_mouse()
{
    //std::cout << MOUSE_VT200            << RESET;
    std::cout << MOUSE_REPORT_BUTTONS   << RESET;
    //std::cout << MOUSE_REPORT_ANY       << RESET;
    std::cout << MOUSE_SGR_EXT_MODE     << RESET << std::flush;
    //std::cout << MOUSE_URXVT_MODE       << RESET << std::flush;

    _flags &= ~use_mouse;

    return rem::code::accepted;
}


void clear()
{
    std::cout << "\x1b[2J";
}


void cursor_off()
{
    std::cout << "\x1b[?25l" << std::flush;
    _flags &= ~st_caret_on;
}


/*!
 * @brief (Re-)activate the screen cursor.
 *
 */
void cursor_on()
{
    std::cout << "\x1b[?25h" << std::flush;
    _flags |= st_caret_on;
}


bool cursor(ui::cxy xy)
{
    if(_geometry.goto_xy(xy))
        std::cout << "\x1b[" << xy.y+1 << ';' << xy.x+1 << 'H'<< std::flush;
    else
    {
        auto l = sys::debug(1);
        l << rem::type::err << color::hotpink3 << " console geometry ["
        << color::yellow << _geometry << color::hotpink3 << "] rejected positioning at:" << color::yellow << xy << l;
        return false;
    }
    return true;
}



void mv(ui::direction::type dir, int d)
{
    char ch = dir == ui::direction::right ? 'C': dir==ui::direction::left  ? 'D' : dir==ui::direction::up    ? 'A' : 'B';
    std::cout << "\x1b[" << d << ch;
}





rem::code render(vchar::pad* blk, ui::cxy xy)
{
    auto l = sys::debug(1); l << "render @" << color::yellow << xy << color::z << l;
    for (int y=0; y < blk->geometry.height(); y++){
        console::render({xy.x, xy.y+y}, (*blk)[{0,y}], blk->geometry.width());
    }
    std::cout << std::flush;
    return rem::code::done;

}


/**
 *
 */
rem::code render(ui::cxy xy, vchar::iterator start, int length)
{
    cursor(xy);//+ui::cxy{1,1});
    auto c = start;
    struct xglyph
    {
        int col{0};
        vchar c{};
    };
    std::stack<xglyph> glyphs;

    auto current_colors = start->colors();
    std::cout << color::render(current_colors);
    for (int i=0; i < length; i++)
    {
        vchar ch = *c++;
        auto  [fg, bg] = ch.colors();
        if(current_colors.bg != bg)
        {
            current_colors.bg = bg;
            std::cout << color::render_background_rgb(current_colors.bg);
        }
        if(current_colors.fg != fg)
        {
            current_colors.fg = fg;
            std::cout << color::render_rgb(current_colors.fg);
        }
        if(ch.d & vchar::UTFBITS)
        {
            if(ch.d & vchar::Frame)
                std::cout << cadre()[ch.frame_id()];
            else
                if(ch.d & vchar::Accent)
                    std::cout << accent_fr::data[ch.accent_id()];
                else
                    if(ch.d & vchar::UGlyph)
                    {
                        //glyphs.push({i,ch});
                        //auto l = sys::debug(1); l << "glyph:" << l << ch.details() << l;
                        std::cout << glyph::data[ch.ascii()] << "\x08";
                    }
        }
        else
            std::cout << ch.ascii();
    }
    // while (!glyphs.empty()){
    //     xglyph& g = glyphs.top();
    //     cursor({xy.x+1+g.col, xy.y+1});
    //     std::cout << glyph::data[g.c.ascii()];
    //     glyphs.pop();
    // }
    return rem::code::done;
}


static io::mouse mev{};

/*!
 * @brief Blocking terminal stdin input listener & loop.
 * @return rem::code
 */
rem::code init_stdinput()
{
    _fd0._id = "fd #" + std::to_string(0);
    _fd0._poll_bits = POLLIN|POLLERR|POLLHUP;
    _fd0._bits = lfd::IMM | lfd::READ;
    _fd0._window_block_size = 4096;
    _fd0._fd = STDIN_FILENO;
    _fd0.init();
    return rem::code::ready;
}


rem::code poll_in()
{
    // auto l = sys::debug(1); l << "poll_in() called." << l;
    _poll_events[0].fd = _fd0._fd;
    _poll_events[0].events = _fd0._poll_bits;

    int nev = poll(_poll_events, 1,-1);
    // l << "number of events: " << nev << l;
    if (nev <= 0)// && (errno != EINTR))
    {
        auto l = sys::error(); l << "poll failed: (events count = " << color::yellow << nev << color::z << "): " << color::deeppink8 <<  strerror(errno) << l;
        return rem::code::rejected;
    }
    for(int n = 0; n < nev; n++){

        if(_poll_events[n].fd == _fd0._fd)
        {
            if (_poll_events[n].revents & POLLIN)
            {
                if(_fd0._read() != rem::action::cont) return rem::code::rejected;
                return stdin_proc();
            }
            if (_poll_events[n].revents & POLLERR)
            {
                auto l = sys::error(); l << "poll failed: (events count = " << color::yellow << nev << color::z << "): " << color::deeppink8 <<  strerror(errno) << l;
                return rem::code::failed;
            }
            if (_poll_events[n].revents & POLLHUP)
            {
                auto l = sys::error(); l << "poll hangup event" << l;
                return rem::code::failed;
            }
        }
    }

    throw sys::exception()[ sys::except() << "system poll seems unable to address the registered STDIN_FILENO. "];
}





//////////////////////////////////////////////////////////////////////
/// \brief terminal::stdin_proc
/// \return rejected or ready;
///
rem::code stdin_proc()
{
    // auto l = sys::debug(1); l << "stdin_proc() called." << l;
    while(!_fd0.empty())
    {
        if(auto rcc = kbhit::parse(_fd0); !!rcc){
            if(_events.back().data.kev.mnemonic == kbhit::ESCAPE){
                return rem::code::ready;
            }
            continue;
        }
        if(auto rcc = mouse::parse(_fd0); !!rcc){
            auto& e = _events.back();
            if(!e.is<mouse>()){
                _fd0.clear(); // discard and dismiss the rest of the input data because it is not parsable.
                return rem::code::rejected;
            }

            mouse::prev_mev = e.data.mev;
            //auto l = sys::status();l << " mouse event: " << e.data.mev() << l;
        }
    }

    return rem::code::ready;
}


} // namespace tux::io
