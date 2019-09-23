#ifndef EXCEPTIONS_H_INCLUDED
#define EXCEPTIONS_H_INCLUDED
#include <exception>
#include <stdexcept>
#include <SDL2/SDL.h>

class IOException: public std::runtime_error
{
public:
	IOException(const std::string& filename, const std::string& what) :
			std::runtime_error((filename + ":" + what).c_str())
	{

	}
};

class SDLException: public std::runtime_error
{
public:
	SDLException(const std::string& filename) :
			std::runtime_error(
					(filename + ":" + std::string(SDL_GetError())).c_str())
	{

	}
};

#endif // EXCEPTIONS_H_INCLUDED
