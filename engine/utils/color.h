#ifndef COLOR_H
#define COLOR_H

#include <SDL2/SDL.h>

namespace utils
{
    namespace color
    {

        const SDL_Color hex2color(const uint32_t hex);
        const uint32_t color2hex(const SDL_Color color);

        const SDL_Color RED = {255, 0, 0, 255};
        const SDL_Color GREEN = {0, 255, 0, 255};
        const SDL_Color BLUE = {0, 0, 255, 255};
        const SDL_Color WHITE = {255, 255, 255, 255};
        const SDL_Color BLACK = {0, 0, 0, 255};

    } // namespace color

} // namespace utils

#endif // COLOR_H
