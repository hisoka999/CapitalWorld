#include "engine/core/gamewindow.h"
#include <iostream>
#include "engine/utils/exceptions.h"
#include <SDL2/SDL_mixer.h>


namespace core
{

GameWindow::GameWindow(const std::string pTitle, int pWidth, int pHeight) :
        title(pTitle), width(pWidth), height(pHeight), win(NULL)
{
    //ctor
}

int GameWindow::open()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        throw SDLException("GameWindow::SDL_Init");
    }
    win = SDL_CreateWindow(title.c_str(), 100, 100, width, height,
            SDL_WINDOW_SHOWN);
    if (win == nullptr)
    {
        throw SDLException("GameWindow::SDL_CreateWindow");
    }
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        throw SDLException("GameWindow::IMG_Init");
    }

    if (TTF_Init() != 0)
    {
        throw SDLException("GameWindow::TTF_Init");
    }


    //Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        throw SDLException(
                "SDL_mixer could not initialize! SDL_mixer Error:"
                        + std::string(Mix_GetError()));
    }

    return 0;
}

GameWindow::~GameWindow()
{
    if (win != nullptr)
    {
        SDL_DestroyWindow (win);
        TTF_Quit();
        IMG_Quit();
        Mix_Quit();
        SDL_Quit();
    }
}
SDL_Window *GameWindow::getSDLWindow()
{
    return win;
}

void GameWindow::delay(unsigned int millsec)
{
    SDL_Delay(millsec);
}

} // namespace core
