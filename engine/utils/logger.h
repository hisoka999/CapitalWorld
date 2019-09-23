#ifndef UTILS_LOGGER_H
#define UTILS_LOGGER_H
#include <string>
#include <SDL2/SDL.h>
#include <ostream>

namespace utils
{

enum class LogLevel
{
	error, warn, info, trace
};

const std::string get_loglevel(LogLevel pLevel);

class Logger
{
public:
	/** Default constructor */
	Logger(LogLevel pLevel);
	Logger();
	/** Default destructor */
	virtual ~Logger();
	void logSDLError(const std::string& msg);
	void error(const std::string& src, const std::string& msg);
	void warn(const std::string& src, const std::string& msg);
	void info(const std::string& src, const std::string& msg);
	void trace(const std::string& src, const std::string& msg);

protected:
	void log(LogLevel pLevel, const std::string& src, const std::string& msg);
private:
	LogLevel level;
	//std::ostream out;
};

} // namespace utils

#endif // UTILS_LOGGER_H
