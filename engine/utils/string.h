/*
 * string.h
 *
 *  Created on: 15.05.2019
 *      Author: stefan
 */

#ifndef UTILS_STRING_H_
#define UTILS_STRING_H_

#include <cstdio>
#include <iostream>
#include <memory>
#include <string>

namespace utils {

const static char* ws = " \t\n\r\f\v";

// trim from end of string (right)
inline std::string& rtrim(std::string& s, const char* t = ws)
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim from beginning of string (left)
inline std::string& ltrim(std::string& s, const char* t = ws)
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from both ends of string (right then left)
inline std::string& trim(std::string& s, const char* t = ws)
{
    return ltrim(rtrim(s, t), t);
}

template <typename T>
T process_arg(T value) noexcept
{
    return value;
}

template <typename T>
T const* process_arg(std::basic_string<T> const& value) noexcept
{
    return value.c_str();
}

template <typename... Args>
std::string string_format(const std::string& format, Args const&... args)
{
    const auto fmt = format.c_str();
    const size_t size = std::snprintf(nullptr, 0, fmt, process_arg(args)...)
        + 1;
    auto buf = std::make_unique<char[]>(size);
    std::snprintf(buf.get(), size, fmt, process_arg(args)...);
    auto res = std::string(buf.get(), buf.get() + size - 1);
    return res;
}

};

#endif /* UTILS_STRING_H_ */
