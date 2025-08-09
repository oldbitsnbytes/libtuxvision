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
#include <tuxvision/utility/io/lfd.h>
#ifdef __APPLE__
   #include <sys/event.h>
#else
   #include <sys/poll.h>
#endif
#include <sys/time.h>

namespace tux::io
{


/**
 * @class listener
 * A class to manage a list of file descriptors and handle epoll-based, kqueue-kevent(BSD) I/O events.

*/
class  listener
{



    lfd::list _fds{};
    int _epoll_fd{-1};
    static constexpr int max_events{10};
#ifdef __APPLE__
    struct kevent _poll_events_kev[listener::max_events]{};
#else
    epoll_event   _poll_events[listener::max_events]{};
#endif

    bool _kill{false};
public:
    listener() = default;
    listener(const std::string& obj_id);
    ~listener();
    std::pair<rem::code, lfd&> attach(lfd&& fds); // ex.: attach({"terminal input channel", 0, 1024, EPOLLIN|EPOLLERR|EPOLLHUP,nullptr});
    rem::code detach(int fnum);
    //std::pair<rem::code, lfd&>  add_fd(lfd&& fds);
    //rem::code init();
    rem::code terminate();

    rem::code run();         ///< blocking loop of polls
    rem::code poll(int _fd); ///< One-shot poll check and consume at once.
    std::pair<rem::code, io::lfd&> query_lfd(int fnum);
    rem::code refresh_fds();
    lfd& operator[](int fd_num);
    rem::code open();
    rem::code close();

};


} // namespace tux::io


/*
 First, create new kqueue:

int kq=kqueue();
Now register your fd in kq:

struct kevent kev;
kev.ident=your_fd;
kev.flags=EV_ADD | EV_CLEAR;
kev.filter=EVFILT_READ;
kev.fflags=0;
kev.data=0;
kev.udata=&your_data;

int res=kevent(kq,&kev,1,0,0,0);
Finally, wait for data to arrive into your socket:

struct kevent res_kevs[5];
int res=kevent(kq,0,0,res_kevs,5,0);
After return, res_kevs[i].ident will contain your socket's descriptor, res_kevs[i].data - number of bytes ready to be read.

See man kevent for more details and features.
*/
