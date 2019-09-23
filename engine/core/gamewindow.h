#ifndef CORE_GAMEWINDOW_H
#define CORE_GAMEWINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>

namespace core {

class GameWindow
{
    public:
        /** Default constructor */
        GameWindow(std::string pTitle,int pWidth,int pHeight);
        /** Default destructor */
        virtual ~GameWindow();

        int open();

        SDL_Window *getSDLWindow();

        void delay(unsigned int millsec);
        int getWidth() { return width; }
        int getHeight() { return height; }
    protected:
    private:
        SDL_Window *win;

        int width;
        int height;
        std::string title;
};

} // namespace core

#endif // CORE_GAMEWINDOW_H
