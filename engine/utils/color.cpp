#include "color.h"

namespace utils
{
    namespace color
    {
        const SDL_Color hex2color(const uint32_t hex)
        {
            SDL_Color color;
            color.r = 0xff0000 & hex;
            color.g = 0xff00 & hex;
            color.b = 0xff & hex;
            color.a = 255;
            return color;
        }

        const uint32_t color2hex(const SDL_Color color)
        {
            uint32_t value = (color.r << 16) + (color.g << 8) + color.b;
            return value;
        }
    }
}