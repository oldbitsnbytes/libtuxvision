#pragma once

#include "sys.h"

template<typename T> sys::out& sys::out::operator << (const T& v)
{
    std::ostringstream o;
    o << v;
    (*ofs) << o.str();
    return *this;
}

template<typename T> rem::code sys::Test::exec(const std::string& sub_test_name, std::function<std::pair<rem::code, T>(sys::Test&)> lambda )
{
    auto log = sys::test(0) << name << "::" << sub_test_name << ":\n";
    log << "Calling the test:" << log;
    auto [r, result] = lambda(*this);
    log << r << log;
    log << result << log;
    return r;
}
