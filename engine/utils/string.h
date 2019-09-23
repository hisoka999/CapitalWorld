/*
 * string.h
 *
 *  Created on: 15.05.2019
 *      Author: stefan
 */

#ifndef UTILS_STRING_H_
#define UTILS_STRING_H_

#include <iostream>
#include <memory>
#include <string>
#include <cstdio>

namespace utils {


template<typename T>
T process_arg(T value) noexcept
{
	return value;
}

template<typename T>
T const * process_arg(std::basic_string<T> const & value) noexcept
{
	return value.c_str();
}

template<typename ... Args>
std::string string_format(const std::string& format, Args const & ... args) {
	const auto fmt = format.c_str();
	const size_t size = std::snprintf(nullptr, 0, fmt, process_arg(args) ...)
			+ 1;
	auto buf = std::make_unique<char[]>(size);
	std::snprintf(buf.get(), size, fmt, process_arg(args) ...);
	auto res = std::string(buf.get(), buf.get() + size - 1);
	return res;
}

}
;


#endif /* UTILS_STRING_H_ */
