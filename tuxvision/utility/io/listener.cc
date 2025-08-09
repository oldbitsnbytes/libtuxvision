//
// Created by oldlonecoder on 25-02-03.
//

#include <tuxvision/utility/io/listener.h>
#include <cerrno>

namespace tux::io
{


static lfd _null{};

listener::listener(const std::string& obj_id){}


listener::~listener()
{
    //_fds.clear();
}




std::pair<rem::code, lfd&> listener::attach(lfd&& fds)
{
    _fds.emplace_back(std::move(fds));
    epoll_event e{};
    auto& fd = _fds.back();
    fd.init();
    e.events = fd._poll_bits;
    e.data.fd = fd._fd;
    //sys::info() << "attach new fd: " << color::yellow << fd._id << color::z << ":" <<//sys::endl;

    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd._fd, &e) < 0)
    {
        //sys::error() << "epoll_ctl() failed: " << std::strerror(errno) << //sys::endl;
        fd._flags.active = 0;
    }
    return {rem::code::accepted, _fds.back()};
}


// std::pair<rem::code, lfd&> listener::add_fd(lfd&& fds)
// {
//     _fds.emplace_back(std::move(fds));
//     //sys::info() << "added " << color::aqua << _fds.back()._id << color::z << //sys::endl;
//     return {rem::code::accepted, _fds.back()};
// }


rem::code listener::detach(int fnum)
{
    auto fdi = std::find_if(_fds.begin(), _fds.end(), [fnum](const auto& fd)->bool
        {
        return fnum == fd._fd;
    });

    if (fdi == _fds.end()) return rem::code::rejected;
    //sys::info() << "detach and remove lfd '" << fdi->_id << color::z << "' from this listener." << //sys::endl;
    epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fdi->_fd, nullptr);
    _fds.erase(fdi);

    return rem::code::accepted;
}



rem::code listener::terminate()
{
    _kill = true;
    return rem::code::accepted;
}


rem::code listener::run()
{

    do
    {
        if (_fds.empty())
        {
            //sys::warning() << color::yellow << "cancelling this listener: fd list is empty()..." << //sys::endl;
            if (_epoll_fd > 2 )
                close();
            return rem::code::rejected;
        }

        auto nevents = epoll_wait(_epoll_fd, _poll_events, listener::max_events,-1);
        //sys::info() << color::yellow << nevents << color::z << " events:" << //sys::endl;
        refresh_fds();
        if ((nevents <= 0) && (errno != EINTR)){
            //sys::error() << "epoll_wait() failed: (events count = " << color::yellow << nevents << color::z << "): " << color::deeppink8 <<  strerror(errno) << //sys::endl;
            return rem::code::failed;
        }
        for (int n = 0; n < nevents; n++){
            for (auto&fd : _fds){
                if (fd._fd ==_poll_events[n].data.fd){
                    if (_poll_events[n].events & EPOLLIN){
                        if (fd._flags.active){
                            auto a = fd._read();
                            ////sys::debug() << color::aqua << fd._id << color::z << ": [" << static_cast<int>(a) << "] " << rem::to_string(a) << //sys::endl;
                            if (a != rem::action::cont){
                                ////sys::info() << "[" << a << "] active lfd to be killed."  << //sys::endl;
                                fd.kill();
                            }
                        }
                        else
                            ;//sys::debug() << " invoked lfd is NOT active." << //sys::endl;
                    }
                    if ((_poll_events[n].events & EPOLLHUP) || (_poll_events[n].events & EPOLLERR)){
                        //sys::info() << " broken link on '" << color::aqua << fd._id << color::z <<  //sys::endl;
                        fd.kill();
                        continue;
                    }
                }
            }
        }
    }
    while (!_kill);

    close();
    return rem::code::done;
}

rem::code listener::poll(int _fd)
{
    auto [r, f] = query_lfd(_fd);
    if(!r){
        //sys::error() << " file descriptor #" << color::red4 << _fd << color::z << " is not registered into this listener's group." << //sys::eol;
        return r;
    }
#ifdef __APPLE__
    auto nevents = kevent(kq,_poll_events_kev,listener::max_events,0)
#else
    auto nevents = epoll_wait(_epoll_fd, _poll_events, listener::max_events,-1);
#endif
    //sys::info() <<  color::yellow << nevents << color::z << " event(s):" << //sys::endl;
    refresh_fds();

    if ((nevents <= 0) && (errno != EINTR)){
        //sys::error() << "epoll_wait() failed: (events count = " << color::yellow << nevents << color::z << "): " << color::deeppink8 <<  strerror(errno) << //sys::endl;
        return rem::code::failed;
    }
    rem::action A{};
    if(f._fd == _poll_events[0].data.fd){
        if (_poll_events[0].events & EPOLLIN)
        {
            if (f._flags.active)
            {
                A = f._read(); // Actual read and process input data signal
                //sys::debug() << color::aqua << f._id << color::z << ": [" << A << "] "  << //sys::endl;
                if (A != rem::action::cont){
                    //sys::info() << "[" << A << "] active lfd to be killed."  << //sys::endl;
                    f.kill();
                    return rem::code::terminate;
                }
            }
            else;
                //sys::debug() << " invoked lfd is NOT active." << //sys::endl;
        }
        else
        {
            if ((_poll_events[0].events & EPOLLHUP) || (_poll_events[0].events & EPOLLERR))
            {
                //sys::info() << " broken link on '" << color::aqua << f._id << color::z <<  //sys::endl;
                f.kill();
                return rem::code::terminate;
            }
        }
        return rem::code::ready;
    }

    return rem::code::ready;
}




std::pair<rem::code, lfd &> listener::query_lfd(int fnum)
{
    for(auto& fd : _fds)
        if(fd._fd == fnum)
            return {rem::code::ready, fd};

    //auto i = std::find_if(_fds.begin(), _fds.end(), [&](const auto& c){ return fnum == c._fd; });
    //if(i != _fds.end()) return {rem::code::ready,*i};

    // Yes returns valid lfd, but we can modify it as we wish - not a good idea. Only temporary solution.
    return {rem::code::rejected, lfd::null_};
}



rem::code listener::refresh_fds()
{
    for (auto& fd : _fds)
    {
        if (fd._flags.kill || fd._flags.del)
        {
            //sys::info() << "removing lfd: '" << color::aqua << fd._id << color::z << //sys::endl;
            if (fd._flags.kill)
                detach(fd._fd);
            else
                epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd._fd, nullptr);
        }
    }
    return rem::code::done;
}


//////////////////////////////////////////////////////
/// \brief listener::operator []
/// \param fd_num
/// \return ref to the lfd instance at the given index num.
///
lfd &listener::operator[](int fd_num)
{
    return _fds[fd_num];
}


rem::code listener::open()
{
    _epoll_fd = epoll_create1(0);
    if (_epoll_fd < 0)
    {
        //sys::error() << "epoll_create1() failed: " << strerror(errno) << //sys::endl;
        return rem::code::rejected;
    }

    //sys::info() << "listener opened: file # " << color::yellow << _epoll_fd << color::z;
    //sys::write() << "ready to run" << //sys::endl;

    return rem::code::ready;
}


rem::code listener::close()
{
    _fds.clear();
    ::close(_epoll_fd);
    //sys::info() << "listener is closed clean." << //sys::endl;
    return rem::code::ok;
}


} // namespace tux::io
