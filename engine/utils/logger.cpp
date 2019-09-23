#include "engine/utils/logger.h"
#include <iostream>
#include <chrono>
#include <ctime>

namespace utils
{
const std::string get_loglevel(LogLevel pLevel)
{
    switch (pLevel)
    {
    case LogLevel::error:
        return "error";
    case LogLevel::info:
        return "info";
    case LogLevel::trace:
        return "trace";
    case LogLevel::warn:
        return "warn";
    default:
        return "";
    }
}

Logger::Logger(LogLevel pLevel) :
        level(pLevel) //, out(std::cerr)
{

}

Logger::Logger() :
        level(LogLevel::trace) //, out(std::cerr)
{

}

void Logger::logSDLError(const std::string& msg)
{
    log(LogLevel::error, msg, SDL_GetError());
}

Logger::~Logger()
{
    //dtor
}

void Logger::log(LogLevel pLevel, const std::string& src,
        const std::string& msg)
{
    char mbstr[100];
    auto time = std::chrono::system_clock::now();
    std::time_t time_t = std::chrono::system_clock::to_time_t(time);
    std::strftime(mbstr, sizeof(mbstr), "%H:%M:%S", std::localtime(&time_t));

    std::cerr << mbstr << "[" << get_loglevel(pLevel) << "]" << src << ":"
            << msg << std::endl;

}

void Logger::error(const std::string& src, const std::string& msg)
{
    log(LogLevel::error, src, msg);
}
void Logger::warn(const std::string& src, const std::string& msg)
{
    log(LogLevel::warn, src, msg);
}
void Logger::info(const std::string& src, const std::string& msg)
{
    log(LogLevel::info, src, msg);
}
void Logger::trace(const std::string& src, const std::string& msg)
{
    log(LogLevel::trace, src, msg);
}

} // namespace utils
